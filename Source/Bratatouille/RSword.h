// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RWeapon.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "RSword.generated.h"

UCLASS()
class BRATATOUILLE_API ARSword : public AActor, public IRWeapon
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARSword();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnUse_Implementation() override;
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* MeshComponentTriggerBox;
	UPROPERTY(VisibleAnywhere)
	USphereComponent* MeshComponentTriggerSphere;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TSubclassOf<UAnimInstance> AnimBlueprintClassSword;
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapBeginSphere(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//// declare overlap end function
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void OnOverlapEndSphere(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void ResetAtk();

	float mCooldown;
	float mOpenTimer;
	float mOpenTimer2;
	bool isAttacking;
	bool canAttack;
	bool mIsOverlapping;
	bool animTimer = false;
	int mDamage;

	FRotator mRot;
	TArray<AActor*> mFoundActors;
};
