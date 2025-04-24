// Fill out your copyright notice in the Description page of Project Settings.


#include "RFarmer.h"
#include "../RBall.h"
ARFarmer::ARFarmer() {
	PrimaryActorTick.bCanEverTick = true;
	m_PV = 500;
	m_Damage = 50;
	m_Speed = 30;
	GetCharacterMovement()->MaxWalkSpeed = m_Speed;
	GetCharacterMovement()->MaxAcceleration = m_Speed * 2;
	m_Cooldown = 3.0f;
	m_ActCooldown = m_Cooldown;
}

void ARFarmer::Tick(float DeltaTime)
{
	//Anna c'est Scooby
	Super::Tick(DeltaTime);
	if (m_ActCooldown <= 0) {
		for (int i = -1; i < 2; i++) {
			FActorSpawnParameters SpawnInfo;
			ARBall* bullet = GetWorld()->SpawnActor<ARBall>(GetTransform().GetLocation() + GetActorForwardVector() * 100 + GetActorRightVector() * i * 100, FRotator(0.f, 0.f, 0.f), SpawnInfo);
			bullet->Init(400.f, m_Damage, (GetActorRotation() + FRotator(0.f, 0.f, 0.f)).Quaternion(), false);
		}
		for (int i = -1; i < 2; i += 2) {
			FActorSpawnParameters SpawnInfo;
			ARWalker* walker = GetWorld()->SpawnActor<ARWalker>(m_Invoc, GetTransform().GetLocation() + GetActorRightVector() * i * 100 + GetActorUpVector() * 10, GetTransform().GetRotation().Rotator(), SpawnInfo);
			if (walker)
				walker->Init(50);
		}
		m_ActCooldown = m_Cooldown;
	}
	m_ActCooldown -= DeltaTime;
}