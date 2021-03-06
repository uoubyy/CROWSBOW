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

UENUM(BlueprintType)
enum class EPowerUpType : uint8 
{
	POWERUP_ADDHEALTH	UMETA(DisplayName = "Add Health"),
	POWERUP_NONE		UMETA(DisplayName = "None")
	// TODO MORE
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	ES_Idle		UMETA(DisplayName = "Idle"),
	ES_Wander	UMETA(DisplayName = "Wander"),
	ES_Running	UMETA(DisplayName = "Running"),
	ES_Attack1	UMETA(DisplayName = "Attack 1"),
	ES_Attack2	UMETA(DisplayName = "Attack 2"),
	ES_Alert	UMETA(DisplayName = "Guard"),
	ES_None		UMETA(DisplayName = "None")
};

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	EDT_OneTime		UMETA(DisplayName = "One Time Damage"),
	EDT_Constant	UMETA(DisplayName = "Constant Damage"),
	EDT_None		UMETA(DisplayName = "None")
};

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	EET_Skeleton	UMETA(DisplayName = "Enemy Skeleton"),
	EET_Dragon		UMETA(DisplayName = "Enemy Dragon"),
	EET_None		UMETA(DisplayName = "None")
};

UENUM(BlueprintType)
enum class EGemType : uint8
{
	EGT_Blue		UMETA(DisplayName = "Gem Blue"),
	EGT_Yellow		UMETA(DisplayName = "Gem Yellow"),
	EGT_Red			UMETA(DisplayName = "Gem Red"),
	EGT_Purple		UMETA(DisplayName = "Gem Purple"),
	EGT_Green		UMETA(DisplayName = "Gem Green")
};

ENUM_RANGE_BY_COUNT(EGemType, EGemType::EGT_Green)

template<typename TEnum>
static FORCEINLINE FString GetEnumValueAsString(const FString& Name, TEnum Value) {
	const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
	if (!enumPtr) return FString("Invalid");
	return enumPtr->GetNameByValue((int64)Value).ToString();
}