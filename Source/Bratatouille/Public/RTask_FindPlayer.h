// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "RTask_FindPlayer.generated.h"
/**
 * 
 */
UCLASS()
class BRATATOUILLE_API URTask_FindPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	URTask_FindPlayer();

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector SelfActorKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector DistanceLeftKey;

	//UPROPERTY(EditAnywhere)
	//FBlackboardKeySelector RunAwayVectKey;

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
