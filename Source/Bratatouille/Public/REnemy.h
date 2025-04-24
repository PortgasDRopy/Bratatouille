// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DmgText.h"
#include "REnemy.generated.h"

UCLASS()
class BRATATOUILLE_API AREnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AREnemy();
	virtual void Init(int niveau);
	void TakeDamage(int damage);
	virtual void Death();
	UFUNCTION()
	void OnDestroy();
	int m_Niveau;
	int m_PV;
	int m_Damage;
	float m_Speed;
	float m_Cooldown;
	float m_ActCooldown;
	int m_XP;
	int m_Materials;
	bool m_IsAlive = true;
	UPROPERTY(BlueprintReadWrite)
	UStaticMeshComponent* m_Halo;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ADmgText> mText;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
