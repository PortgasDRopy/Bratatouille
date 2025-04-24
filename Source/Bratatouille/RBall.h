// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "RBall.generated.h"

UCLASS()
class BRATATOUILLE_API ARBall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARBall();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ballMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCapsuleComponent* capsuleCollider;
	AActor* BallActor;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Init(float fSpeed, int iDamage, FQuat vBeginOrientation,bool shotByPlayer);
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void KillSelf();

private:
	int _iDamage;
	float mSpeed;
	bool mHasHit;
	bool mShotByPlayer;
	UMaterialInterface* NewMaterial;
};
