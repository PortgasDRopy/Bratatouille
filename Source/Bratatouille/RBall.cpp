// Fill out your copyright notice in the Description page of Project Settings.


#include "RBall.h"
#include "BratatouilleCharacter.h"
#include "REnemy.h"

// Sets default values
ARBall::ARBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ballMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball"));
	UStaticMesh* cubeMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Script/Engine.StaticMesh'/Game/Ball/Mesh/Bullet_Bill.Bullet_Bill''")).Object;
	ballMesh->SetStaticMesh(cubeMesh);
	ballMesh->SetupAttachment(RootComponent);
	mHasHit = false;
	//ballMesh->SetSimulatePhysics(true);
	//ballMesh->SetEnableGravity(false);
	//ballMesh->SetNotifyRigidBodyCollision(true);
	capsuleCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollider"));
	capsuleCollider->SetupAttachment(ballMesh); 

	capsuleCollider->SetCapsuleSize(25.f, 50.f);
	capsuleCollider->SetRelativeRotation(FRotator(90.f, 0.f,0.f));

	capsuleCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	capsuleCollider->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	capsuleCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	capsuleCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	ballMesh->SetSimulatePhysics(false);
	ballMesh->SetEnableGravity(false);
	ballMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	NewMaterial = ConstructorHelpers::FObjectFinder<UMaterialInterface>(TEXT("Material'/Game/Enemy/models/NewMaterial.NewMaterial'")).Object;

}

// Called when the game starts or when spawned
void ARBall::BeginPlay()
{
	Super::BeginPlay();

	//ballMesh->OnComponentHit.AddUniqueDynamic(this, &ARBall::OnHit);

	FTimerHandle timerHandle;
	GetWorldTimerManager().SetTimer(timerHandle, this, &ARBall::KillSelf, 3, false);

}

// Called every frame
void ARBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorWorldOffset(-GetActorForwardVector() * mSpeed * DeltaTime);
	ballMesh->AddLocalRotation(FRotator(0.f,0.f,3.f) * 100 * DeltaTime);
}

void ARBall::Init(float fSpeed, int iDamage, FQuat vBeginOrientation, bool shotByPlayer)
{
	_iDamage = iDamage;
	mSpeed = fSpeed;
	SetActorRotation(vBeginOrientation);
	mShotByPlayer = shotByPlayer;
	ballMesh->AddWorldRotation(FRotator(0.f, 180.f, 0.f));
	capsuleCollider->OnComponentBeginOverlap.AddDynamic(this, &ARBall::OnOverlapBegin);
	TArray<AActor*> overlappingActors;
	capsuleCollider->GetOverlappingActors(overlappingActors);
	if(mShotByPlayer == false)
	{
		
		if (NewMaterial)
		{
			ballMesh->SetMaterial(0, NewMaterial); // Setting the first material slot (index 0)
		}
	}
	// Check for overlap with enemy
	for (AActor* actor : overlappingActors)
	{
		if(mShotByPlayer)
		{
			AREnemy* enemy = Cast<AREnemy>(actor);
			if (enemy)
			{
				// Apply damage or handle the interaction
				enemy->TakeDamage(_iDamage);
				KillSelf();
			}
		}
		else {
			ABratatouilleCharacter* player = Cast<ABratatouilleCharacter>(actor);
			if (player)
			{
				player->TakeDamage(_iDamage);
				KillSelf();
			}
		}
	}

	//ballMesh->AddForce(ballMesh->GetForwardVector() * fSpeed);
}

void ARBall::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(mHasHit == false)
	{
		ABratatouilleCharacter* player = Cast<ABratatouilleCharacter>(OtherActor);
		if (player)
		{
			//player->TakeDamage(_iDamage);
		}
		else {
			AREnemy* enemy = Cast<AREnemy>(OtherActor);
			if (enemy)
			{
				enemy->TakeDamage(_iDamage);
			}
		}
		mHasHit = true;
		KillSelf();
	}
}

void ARBall::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (mShotByPlayer)
	{
		AREnemy* enemy = Cast<AREnemy>(OtherActor);
		if (enemy)
		{
			// Apply damage or handle the interaction
			enemy->TakeDamage(_iDamage);
			KillSelf();
		}
	}
	else {
		ABratatouilleCharacter* player = Cast<ABratatouilleCharacter>(OtherActor);
		if (player)
		{
			player->TakeDamage(_iDamage);
			KillSelf();
		}
	}
}

void ARBall::KillSelf()
{
	SetActorEnableCollision(false);
	Destroy();
}

