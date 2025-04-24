// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "REnemy.h"

#include "RWalker.generated.h"
class ABratatouilleCharacter;
/**
 * 
 */
UCLASS()
class BRATATOUILLE_API ARWalker : public AREnemy
{
	GENERATED_BODY()
public:
	ARWalker();
	virtual void Init(int niveau) override;
	virtual void Tick(float DeltaTime) override;
	ABratatouilleCharacter* player;
protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	bool mIsOverlapping;
	float mOpenTimer;
	bool mCanAttack;
};
