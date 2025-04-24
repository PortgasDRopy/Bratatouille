// Fill out your copyright notice in the Description page of Project Settings.

#include "RTask_FindPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AiController.h"

URTask_FindPlayer::URTask_FindPlayer()
{
	NodeName = TEXT("Find Player");
}

EBTNodeResult::Type URTask_FindPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();   
	MyBlackboard->SetValueAsObject(SelfActorKey.SelectedKeyName, OwnerComp.GetAIOwner()->GetPawn());
	MyBlackboard->SetValueAsObject(TargetActorKey.SelectedKeyName, GetWorld()->GetFirstPlayerController()->GetPawn());
	AActor* TActor = Cast<AActor>(MyBlackboard->GetValueAsObject(TargetActorKey.SelectedKeyName));
	AActor* SActor = Cast<AActor>(MyBlackboard->GetValueAsObject(SelfActorKey.SelectedKeyName));
	MyBlackboard->SetValueAsFloat(DistanceLeftKey.SelectedKeyName, SActor->GetDistanceTo(TActor));
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}