// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "REnemy.h"
#include "RWalker.h"
#include "RFarmer.generated.h"

/**
 * 
 */
UCLASS()
class BRATATOUILLE_API ARFarmer : public AREnemy
{
	GENERATED_BODY()
public :
	ARFarmer();
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)
	TSubclassOf<ARWalker> m_Invoc;
};
