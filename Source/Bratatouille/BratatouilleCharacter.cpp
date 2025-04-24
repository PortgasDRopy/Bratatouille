// Copyright Epic Games, Inc. All Rights Reserved.

#include "BratatouilleCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "RWeapon.h"
#include "RSword.h"
#include "RGun.h"
#include "RGameState.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ABratatouilleCharacter

ABratatouilleCharacter::ABratatouilleCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = 1200.0f; // The camera follows at this distance behind the character	
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	//CameraBoom->SetWorldLocation(FVector(0.f, 0.f, 600.f));
	CameraBoom->bDoCollisionTest = false;

	////CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


	//FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	////FollowCamera->SetupAttachment(RootComponent);
	//FollowCamera->SetRelativeLocation(FVector(430.f, 1640.f, 2270.f));
	//FollowCamera->SetRelativeRotation(FRotator(-70.f, 0.f, 0.f));
	//FollowCamera->bUsePawnControlRotation = false; 
	//FollowCamera->FieldOfView = 90.f;

	mPos.Add(FVector(250.f, 150.f, 0.f));
	mPos.Add(FVector(250.f, -150.f, 0.f));
	mPos.Add(FVector(0.f, 250.f, 0.f));
	mPos.Add(FVector(0.f, -250.f, 0.f));
	mPos.Add(FVector(-250.f, 150.f, 0.f));
	mPos.Add(FVector(-250.f, -150.f, 0.f));

	mRot.Add(FRotator(0.f,45.f, 0.f));
	mRot.Add(FRotator(0.f,360.f-45.f, 0.f));
	mRot.Add(FRotator(0.f,90.f, 0.f));
	mRot.Add(FRotator(0.f,360.f - 90.f, 0.f));
	mRot.Add(FRotator(0.f,135.f, 0.f));
	mRot.Add(FRotator(0.f,360.f - 135.f, 0.f));
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	mMaxHp = 100;
	mHp = mMaxHp;
	mSpeed = 500;
	mDamageMultiplier = 1.f;
	mCooldown = 1.f;
	mOppenheimer = 0.f;
}


void ABratatouilleCharacter::TakeDamage(int iDamage)
{
	mHp -= iDamage;
	ARGameState::SetHp(this, mHp);
	if (mHp <= 0)
	{
		OnDeath();
	}
}

void ABratatouilleCharacter::OnDeath()
{
	//Death function
	//GEngine->AddOnScreenDebugMessage(0, 15.0f, FColor::Black, TEXT("u r dead"));
	ARGameState::Death(this);
}

void ABratatouilleCharacter::Tick(float DeltaTime)
{
	if (mHp < mMaxHp) {
		mOppenheimer += DeltaTime;
		if (mOppenheimer >= mCooldown) {
			mOppenheimer -= mCooldown;
			mHp += 1;
			ARGameState::SetHp(this, mHp);
		}
	}
}

void ABratatouilleCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	UClass* SwordBP = StaticLoadClass(AActor::StaticClass(), nullptr, TEXT("/Game/Characters/MyRSword.MyRSword_C"));

	if (SwordBP)
	{
		FActorSpawnParameters SpawnInfo;
		AActor* SpawnedSword = GetWorld()->SpawnActor<AActor>(SwordBP, FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f), SpawnInfo);
		AddWeapon(Cast<ARSword>(SpawnedSword));
	}

}

void ABratatouilleCharacter::AddHp(int hp)
{
	mMaxHp += hp;
	//ARGameState::SetMouseForGame(this);
	ARGameState::SetMaxHp(this, mMaxHp);
}

void ABratatouilleCharacter::AddSpeed(int speed)
{
	mSpeed += speed;
	GetCharacterMovement()->MaxWalkSpeed = mSpeed;
	//ARGameState::SetMouseForGame(this);
}

void ABratatouilleCharacter::AddDamage(float damage)
{
	mDamageMultiplier += damage;
	//ARGameState::SetMouseForGame(this);
}

//////////////////////////////////////////////////////////////////////////
// Input

void ABratatouilleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABratatouilleCharacter::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ABratatouilleCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABratatouilleCharacter::Look);

		// Dashing
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &ABratatouilleCharacter::Dash);
		// Using
		EnhancedInputComponent->BindAction(UseAction, ETriggerEvent::Started, this, &ABratatouilleCharacter::Use);
		// Pausing
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &ABratatouilleCharacter::Pause);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ABratatouilleCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FVector(1.f, 0.f, 0.f);

		// get right vector 
		const FVector RightDirection = FVector(0.f, 1.f, 0.f);

		// add movement 
		AddMovementInput(ForwardDirection , MovementVector.Y * mSpeed);
		AddMovementInput(RightDirection , MovementVector.X * mSpeed);
	}
}

void ABratatouilleCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ABratatouilleCharacter::Dash(const FInputActionValue& Value)
{
	if (canDash)
	{
		canDash = false;
		LaunchCharacter(FVector(MovementVector.Y, MovementVector.X,0) * 2000, true, true);
		
		GetWorldTimerManager().SetTimer(timerHandle, this, &ABratatouilleCharacter::ResetDash, 1, false);
	}
}

void ABratatouilleCharacter::ResetDash()
{
	canDash = true; 
	GetWorldTimerManager().ClearTimer(timerHandle);
}

void ABratatouilleCharacter::ResetMove()
{
	MovementVector = FVector2D::Zero();
}
void ABratatouilleCharacter::Use() {
	for (int i = 0; i < mWeapons.Num();i++) {
		IRWeapon::Execute_OnUse(mWeapons[i]);
	}
}

void ABratatouilleCharacter::Pause()
{
	ARGameState::OpenMenu(this);
}

void ABratatouilleCharacter::AddWeapon(AActor* weapon)
{
	if(weapon->Implements<URWeapon>() && mWeapons.Num() < 6)
	{
		mWeapons.Emplace(weapon);
		weapon->SetActorLocation(weapon->GetActorTransform().GetLocation() + mPos[mWeapons.Num()-1]);
		weapon->SetActorRotation(weapon->GetActorRotation() + mRot[mWeapons.Num() - 1]);

	}
	else {
		weapon->Destroy();
	}

}


