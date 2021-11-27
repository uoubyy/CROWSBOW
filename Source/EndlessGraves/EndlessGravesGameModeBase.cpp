// Copyright Epic Games, Inc. All Rights Reserved.


#include "EndlessGravesGameModeBase.h"
#include "EndlessGravesCharacter.h"
#include "UObject/ConstructorHelpers.h"

AEndlessGravesGameModeBase::AEndlessGravesGameModeBase()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/EndlessGraves/Core/BP_Gerald"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}