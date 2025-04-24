// Fill out your copyright notice in the Description page of Project Settings.


#include "DmgText.h"
#include <Camera/CameraComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>


// Sets default values
ADmgText::ADmgText()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	dmgText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("DmgText"));
}

// Called when the game starts or when spawned
void ADmgText::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle timerHandle;
	GetWorldTimerManager().SetTimer(timerHandle, this, &ADmgText::SelfDestroy, 1, false);
}

// Called every frame
void ADmgText::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UCameraComponent* playerCam = UGameplayStatics::GetPlayerPawn(this, 0)->GetComponentByClass<UCameraComponent>();
	FRotator TextRot = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation() + FVector(0,0,50), playerCam->GetComponentLocation());
	this->SetActorRotation(TextRot);
}

void ADmgText::Init(int value)
{
	dmgText->SetText(FText::FromString(FString::Printf(TEXT("%d"), value)));
}

void ADmgText::SelfDestroy()
{
	Destroy();
}

