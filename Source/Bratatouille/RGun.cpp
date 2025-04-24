// Fill out your copyright notice in the Description page of Project Settings.


#include "RGun.h"
#include <Kismet/KismetMathLibrary.h>
#include "REnemy.h"
#include <Kismet/GameplayStatics.h>
#include "BratatouilleCharacter.h"

// Sets default values
ARGun::ARGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun"));
	MeshComponentTriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerRange"));
	UStaticMesh* cubeMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/Characters/Mannequin_UE4/Meshes/bullet_bill_blaster.bullet_bill_blaster'")).Object;
	MeshComponentTriggerSphere->SetupAttachment(RootComponent);
	MeshComponentTriggerSphere->SetSphereRadius(800.f);
	MeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	MeshComponent->SetStaticMesh(cubeMesh);
	MeshComponent->SetupAttachment(MeshComponentTriggerSphere);
	MeshComponent->AddLocalOffset(FVector(-100.f, 0.f, 0.f));
	MeshComponent->AddWorldRotation(FRotator(0.f, 90.f, 0.f));
	mCanShoot = true;
	mCooldown = 1.0f;
	mOpenTimer = 0.f;

}

// Called when the game starts or when spawned
void ARGun::BeginPlay()
{
	Super::BeginPlay();
	FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::SnapToTarget, false);
	AttachToActor(GetWorld()->GetFirstPlayerController()->GetPawn(), rules);
	MeshComponentTriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &ARGun::OnOverlapBeginSphere);
	MeshComponentTriggerSphere->OnComponentEndOverlap.AddDynamic(this, &ARGun::OnOverlapEndSphere);
	TArray<AActor*> overlappingActors;
	MeshComponentTriggerSphere->GetOverlappingActors(overlappingActors);

	// Check for overlap with enemy
	for (AActor* actor : overlappingActors)
	{
		AREnemy* enemy = Cast<AREnemy>(actor);
		if (enemy)
		{
			mIsOverlapping = true;
			break; 
		}
	}
}

void ARGun::OnUse_Implementation()
{
	if(mCanShoot)
	{
		FActorSpawnParameters SpawnInfo;
		ARBall* bullet = GetWorld()->SpawnActor<ARBall>(MeshComponent->GetComponentLocation()/* + MeshComponent->GetForwardVector() * 100*/, FRotator(0.f, 0.f, 0.f), SpawnInfo);
		AActor* player = UGameplayStatics::GetPlayerPawn(this, 0);
		ABratatouilleCharacter* character = Cast<ABratatouilleCharacter>(player);
		if(character)
		{
			bullet->Init(400.f, 10 * character->mDamageMultiplier, (GetActorRotation() + FRotator(0.f, 0.f, 0.f)).Quaternion()/*FRotator(0.f, 0.f, 0.f).Quaternion()*/, true);
		}
		mCanShoot = false;
	}
}

// Called every frame
void ARGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (mCanShoot == false) {
		mOpenTimer += DeltaTime;
		if (mOpenTimer >= mCooldown) {
			mCanShoot = true;
			mOpenTimer = 0.f;
		}
	}
	if (mIsOverlapping) {
		FVector minForward = FVector(1000.f, 1000.f, 1000.f);
		for (int i = 0;i < mFoundActors.Num();i++) {
			if(IsValid(mFoundActors[i]))
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

}

void ARGun::OnOverlapBeginSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AREnemy* enemy = Cast<AREnemy>(OtherActor);
	if (enemy) {
		mIsOverlapping = true;

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AREnemy::StaticClass(), mFoundActors);
	}
}

void ARGun::OnOverlapEndSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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

