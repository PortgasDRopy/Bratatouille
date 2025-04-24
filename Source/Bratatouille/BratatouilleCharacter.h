// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "BratatouilleCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class RWeapon;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ABratatouilleCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Dash Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DashAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* UseAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PauseAction;


	TArray<FVector> mPos;
	TArray<FRotator> mRot;
	float mCooldown;
	float mOppenheimer;
	//UObject* mSword;

public:
	ABratatouilleCharacter();
	
	void TakeDamage(int iDamage);

	void OnDeath();
	virtual void Tick(float DeltaTime) override;
protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for looking input */
	void Dash(const FInputActionValue& Value);

	void ResetDash();

	void ResetMove();
	
	void Use();

	void Pause();


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	UFUNCTION(BlueprintCallable)
	void AddHp(int hp);
	UFUNCTION(BlueprintCallable)
	void AddSpeed(int speed);
	UFUNCTION(BlueprintCallable)
	void AddDamage(float damage);
	UFUNCTION(BlueprintCallable)
	void AddWeapon(AActor* weapon);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int mHp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int mMaxHp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int mSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float mDamageMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = WeaponArray, meta = (AllowPrivateAccess = "true"))
	TArray<UObject*> mWeapons;

private:
	
	bool canDash = true;
	FTimerHandle timerHandle;
	FVector2D MovementVector;
};

