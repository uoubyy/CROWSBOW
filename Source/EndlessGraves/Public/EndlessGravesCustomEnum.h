// Copyright EndlessGraves, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "EndlessGravesCustomEnum.generated.h"


UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	WEAPON_ARROW UMETA(DisplayName = "Arrow"),
	WEAPON_SWORD UMETA(DisplayName = "Sword")
};

template<typename TEnum>
static FORCEINLINE FString GetEnumValueAsString(const FString& Name, TEnum Value) {
	const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
	if (!enumPtr) return FString("Invalid");
	return enumPtr->GetNameByValue((int64)Value).ToString();
}