// Copyright Epic Games, Inc. All Rights Reserved.

#include "BratatouilleGameMode.h"
#include "BratatouilleCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABratatouilleGameMode::ABratatouilleGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
