// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RWeapon.h"
#include "RBall.h"
#include "Components/SphereComponent.h"
#include "RGun.generated.h"

UCLASS()
class BRATATOUILLE_API ARGun : public AActor, public IRWeapon
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnUse_Implementation() override;
public:	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;
	UPROPERTY(VisibleAnywhere)
	USphereComponent* MeshComponentTriggerSphere;
	bool mIsOverlapping;

	FRotator mRot;
	TArray<AActor*> mFoundActors;
	float mCooldown;
	bool mCanShoot;
	float mOpenTimer;
	//UPROPERTY(EditDefaultsOnly)
	//TSubclassOf<ARBall> mBullet;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void OnOverlapBeginSphere(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEndSphere(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
