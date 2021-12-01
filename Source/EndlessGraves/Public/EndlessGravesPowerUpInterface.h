// Copyright EndlessGraves, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EndlessGravesCustomEnum.h"
#include "EndlessGravesPowerUpInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEndlessGravesPowerUpInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ENDLESSGRAVES_API IEndlessGravesPowerUpInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual EPowerUpType GetPowerUpType() const { return EPowerUpType::POWERUP_NONE; }

	virtual float GetPowerUpEffectValue() const{ return 0.0f; }
};
