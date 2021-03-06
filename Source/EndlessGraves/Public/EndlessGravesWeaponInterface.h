// Copyright EndlessGraves, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EndlessGravesCustomEnum.h"
#include "EndlessGravesWeaponInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEndlessGravesWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ENDLESSGRAVES_API IEndlessGravesWeaponInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual float GetDamage() const { return 0.0f; }

	virtual EDamageType GetDamageType() const { return EDamageType::EDT_OneTime; }
};
