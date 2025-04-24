// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "REnemy.h"
#include "RShooter.generated.h"

/**
 * 
 */
UCLASS()
class BRATATOUILLE_API ARShooter : public AREnemy
{
	GENERATED_BODY()
public:
	ARShooter();
	virtual void Init(int niveau) override;
	virtual void Tick(float DeltaTime) override;
};
