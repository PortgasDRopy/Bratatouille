// Fill out your copyright notice in the Description page of Project Settings.


#include "REnemy.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../RGameState.h"
// Sets default values
AREnemy::AREnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AREnemy::Init(int niveau)
{
	m_Niveau = niveau;
	m_Halo->SetVisibility(false);
}

void AREnemy::TakeDamage(int damage) 
{
	m_PV -= damage;

	if (mText)
	{
		ADmgText* text = GetWorld()->SpawnActor<ADmgText>(mText, this->ActorToWorld());
		text->Init(damage);
	}

	if (m_PV <= 0 && m_IsAlive == true) {
		Death();
	}
}

void AREnemy::Death() 
{
	m_IsAlive = false;
	m_Halo->SetVisibility(true);

	if (GetController())
	{
		GetController()->UnPossess();
	}
	if (GetMesh())
	{
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
		GetMesh()->PlayAnimation(nullptr, false);
	}
	AddActorLocalOffset(FVector(0, 0, 250));
	ARGameState::AddExp(this, m_XP);
	ARGameState::AddCurrency(this,m_Materials);
	ARGameState::RemoveFromLifeEnemyList(this, this);
	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(Handle, this, &AREnemy::OnDestroy, 1, false);
}

void AREnemy::OnDestroy() 
{
	GetWorldTimerManager().ClearAllTimersForObject(this);
	Destroy();
}

void AREnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AREnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AREnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

