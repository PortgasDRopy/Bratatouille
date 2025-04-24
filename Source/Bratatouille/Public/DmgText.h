// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/TextRenderComponent.h>
#include "DmgText.generated.h"

UCLASS()
class BRATATOUILLE_API ADmgText : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADmgText();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextRenderComponent* dmgText;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Init(int value);

	void SelfDestroy();
};
