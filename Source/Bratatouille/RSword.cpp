// Fill out your copyright notice in the Description page of Project Settings.


#include "RSword.h"
#include "REnemy.h"
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>
#include "AnimBP.h"
#include "BratatouilleCharacter.h"


// Sets default values
ARSword::ARSword()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Sword"));
	USkeletalMesh* cubeMesh = ConstructorHelpers::FObjectFinder<USkeletalMesh>(TEXT("SkeletalMesh'/Game/AttackSystem/Models/ironclads-sword/source/SKM_Sword.SKM_Sword'")).Object;
	MeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	MeshComponent->SetSkeletalMesh(cubeMesh);
	//MeshComponent->SetAnimInstanceClass()
	MeshComponentTriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerRange"));
	//MeshComponentTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerAttack"));
	//ConstructorHelpers::FObjectFinder<UAnimBlueprint> Anim(TEXT("/Game/AttackSystem/Models/ironclads-sword/source/SwordAnimController1.SwordAnimController1"));
	//if (Anim.Succeeded())
	//{
	//	MeshComponent->SetAnimInstanceClass(Anim.Object->GeneratedClass);
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Error, TEXT("Failed to load AnimBlueprint"));
	//}

	
	MeshComponentTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	/*UStaticMesh* cubeMesh1 = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")).Object;
	MeshComponentTrigger->SetStaticMesh(cubeMesh1);*/
	MeshComponentTriggerSphere->SetupAttachment(RootComponent);
	MeshComponentTriggerSphere->SetSphereRadius(400.f);
	MeshComponentTriggerBox->SetBoxExtent(FVector(128.f, 128.f, 128.f));
	MeshComponent->SetupAttachment(MeshComponentTriggerSphere);
	MeshComponentTriggerBox->SetupAttachment(MeshComponent);
	MeshComponent->AddLocalOffset(FVector(-100.f, 0.f, 0.f));
	MeshComponentTriggerBox->AddLocalOffset(FVector(0.f, 100.f, 0.f));
	MeshComponentTriggerBox->AddWorldRotation(FRotator(0.f,0.f,30.f));
	MeshComponentTriggerBox->AddLocalOffset(FVector(0.f, 100.f, 0.f));
	MeshComponent->SetSimulatePhysics(false);
	MeshComponentTriggerBox->SetGenerateOverlapEvents(false);
	mIsOverlapping = false;
	mDamage = 10;
}

// Called when the game starts or when spawned
void ARSword::BeginPlay()
{
	Super::BeginPlay();
	if (AnimBlueprintClassSword)
	{
		MeshComponent->SetAnimInstanceClass(AnimBlueprintClassSword);
	}
	FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::SnapToTarget, false);
	AttachToActor(GetWorld()->GetFirstPlayerController()->GetPawn(), rules);
	MeshComponent->AddWorldRotation(FRotator(0.f, 360.f - 90.f, -30.f));
	MeshComponentTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ARSword::OnOverlapBegin);
	MeshComponentTriggerBox->OnComponentEndOverlap.AddDynamic(this, &ARSword::OnOverlapEnd);
	MeshComponentTriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &ARSword::OnOverlapBeginSphere);
	MeshComponentTriggerSphere->OnComponentEndOverlap.AddDynamic(this, &ARSword::OnOverlapEndSphere);
	mCooldown = 1.f;
	mOpenTimer = 0.f;
	mOpenTimer2 = 0.f;
	isAttacking = false;
	canAttack = true;
}


// Called every frame
void ARSword::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);
	if (mIsOverlapping) {
		FVector minForward = FVector(1000.f, 1000.f, 1000.f);
		for (int i = 0;i < mFoundActors.Num();i++) {
			if (IsValid(mFoundActors[i]))
			{
				AREnemy* enemy = Cast<AREnemy>(mFoundActors[i]);
				if (enemy)
				{
					if (enemy->m_IsAlive) {
						FVector Forward = mFoundActors[i]->GetTransform().GetLocation() - GetActorLocation();
						if (Forward.Size() < minForward.Size())
						{
							minForward = Forward;
							mRot = UKismetMathLibrary::MakeRotFromXZ(Forward, FVector(0.f, 0.f, 1.f));
						}
					}
				}
			}
			else {
				AActor* actor = mFoundActors[i];
				mFoundActors.Remove(actor);
			}
		}
		SetActorRotation(mRot);
	}
	if (isAttacking) {
		mOpenTimer += DeltaTime;
		if (mOpenTimer >= 0.5f) {
			mOpenTimer = 0.f;
			isAttacking = false;
			MeshComponentTriggerBox->SetGenerateOverlapEvents(false);
		}
		else {
			//MeshComponent->AddWorldRotation(FRotator(0.f,1.f,0.f) * DeltaTime * 100);
		}
	}
	else if (isAttacking == false && canAttack == false) {
		mOpenTimer2 += DeltaTime;
		if (mOpenTimer2 >= mCooldown) {
			mOpenTimer2 = 0.f;
			canAttack = true;
		}
	}
	//MeshComponent->SetWorldRotation(MeshComponent->GetRelativeTransform().Rotator() + (FRotator(0.f, 1.f, 0.f)*DeltaTime));
}

void ARSword::OnUse_Implementation()
{
	if(canAttack)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Attack!"));
		MeshComponentTriggerBox->SetGenerateOverlapEvents(true);
		isAttacking = true;
		canAttack = false;

		if (!animTimer)
		{
			animTimer = true;
			if (AnimBlueprintClassSword)
			{
				UAnimBP* anim = Cast<UAnimBP>(MeshComponent->GetAnimInstance());
				anim->isAtk = true;
			}
			FTimerHandle timerHandle;
			GetWorldTimerManager().SetTimer(timerHandle, this, &ARSword::ResetAtk, 0.3, false);
		}
	}
}

void ARSword::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AREnemy* enemy = Cast<AREnemy>(OtherActor);
	if (enemy) {
		AActor* player = UGameplayStatics::GetPlayerPawn(this, 0);
		ABratatouilleCharacter* character = Cast<ABratatouilleCharacter>(player);
		if (character) {
			enemy->TakeDamage(mDamage * character->mDamageMultiplier);
		}
	}
}

void ARSword::OnOverlapBeginSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AREnemy* enemy = Cast<AREnemy>(OtherActor);
	if (enemy) {
	mIsOverlapping = true;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AREnemy::StaticClass(), mFoundActors);

	}
}

void ARSword::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void ARSword::OnOverlapEndSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AREnemy* enemy = Cast<AREnemy>(OtherActor);
	if (enemy) {
		mIsOverlapping = false;
		TArray<AActor*> overlappingActors;
		MeshComponentTriggerSphere->GetOverlappingActors(overlappingActors);

		// Check for overlap with enemy
		for (AActor* actor : overlappingActors)
		{
			AREnemy* enemy1 = Cast<AREnemy>(actor);
			if (enemy1)
			{
				mIsOverlapping = true;
				break;
			}
		}
	}
}


void ARSword::ResetAtk()
{
	if (AnimBlueprintClassSword)
	{
		UAnimBP* anim = Cast<UAnimBP>(MeshComponent->GetAnimInstance());
		anim->isAtk = false;
	}
	GetWorldTimerManager().ClearAllTimersForObject(this);
	animTimer = false;
}

