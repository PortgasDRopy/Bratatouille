// Fill out your copyright notice in the Description page of Project Settings.


#include "RShooter.h"
#include "../RBall.h"

ARShooter::ARShooter() {
	PrimaryActorTick.bCanEverTick = true;
}

void ARShooter::Init(int niveau)
{
	Super::Init(niveau);
	m_PV = 1 * m_Niveau;
	m_Damage = 2 * m_Niveau;
	m_Speed = 20 * m_Niveau;
	GetCharacterMovement()->MaxWalkSpeed = m_Speed;
	GetCharacterMovement()->MaxAcceleration = m_Speed * 2;
	m_Cooldown = 2.0f;
	m_ActCooldown = 0;
	m_XP = 10 * m_Niveau;
	m_Materials = 2 * m_Niveau;
}

void ARShooter::Tick(float DeltaTime) 
{
	//Anna c'est Scooby
	Super::Tick(DeltaTime);
	if (m_ActCooldown <= 0) {
		FActorSpawnParameters SpawnInfo;
		ARBall* bullet = GetWorld()->SpawnActor<ARBall>(GetTransform().GetLocation() + GetActorForwardVector() * 100, FRotator(0.f, 0.f, 0.f), SpawnInfo);
		bullet->Init(400.f, m_Damage, (GetActorRotation() + FRotator(0.f, 0.f, 0.f)).Quaternion(), false);
		m_ActCooldown = m_Cooldown;
	}
	m_ActCooldown -= DeltaTime;
}