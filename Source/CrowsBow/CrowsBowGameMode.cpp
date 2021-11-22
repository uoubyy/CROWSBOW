// Copyright Epic Games, Inc. All Rights Reserved.

#include "CrowsBowGameMode.h"
#include "CrowsBowCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACrowsBowGameMode::ACrowsBowGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/CrowsBow/Blueprints/MainCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
