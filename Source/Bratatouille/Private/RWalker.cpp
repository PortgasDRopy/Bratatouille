// Fill out your copyright notice in the Description page of Project Settings.

#include "RWalker.h"
#include "Components/CapsuleComponent.h"
#include "../BratatouilleCharacter.h"
ARWalker::ARWalker() {
	PrimaryActorTick.bCanEverTick = true;
	mIsOverlapping = false;
	mOpenTimer = 0.f;
	mCanAttack = true;
}

void ARWalker::Init(int niveau)
{
	Super::Init(niveau);
	m_PV = 2 * m_Niveau;
	m_Damage = 1 * m_Niveau;
	m_Speed = 10 * m_Niveau;
	GetCharacterMovement()->MaxWalkSpeed = m_Speed;
	GetCharacterMovement()->MaxAcceleration = m_Speed * 2;
	m_Cooldown = 0.4f;
	m_ActCooldown = m_Cooldown;
	m_XP = 20 * m_Niveau;
	m_Materials = 5 * m_Niveau;
}

void ARWalker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (mIsOverlapping && mCanAttack) {
		mCanAttack = false;
		player->TakeDamage(m_Damage);

	}
	if (mCanAttack == false) {
		mOpenTimer += DeltaTime;
		if (mOpenTimer >= m_Cooldown) {
			mOpenTimer -= m_Cooldown;
			mCanAttack = true;
		}
	}
}

void ARWalker::BeginPlay()
{
	Super::BeginPlay();
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ARWalker::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ARWalker::OnOverlapEnd);
}

void ARWalker::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABratatouilleCharacter* player1 = Cast<ABratatouilleCharacter>(OtherActor);
	if (player1)
	{
		player = player1;
		player->TakeDamage(m_Damage);
		mIsOverlapping = true;
	}
}

void ARWalker::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABratatouilleCharacter* player1 = Cast<ABratatouilleCharacter>(OtherActor);
	if (player1)
	{
		player = nullptr;
		//player->TakeDamage(m_Damage);
		mIsOverlapping = false;
	}
}
