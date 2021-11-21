//  Copyright Yanyi Bao. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM()
enum class EnemyType : uint8
{
	ET_BAT     UMETA(DisplayName = "Bat")
};

UENUM(BlueprintType)
enum class WeaponType : uint8
{
	WP_ARROW UMETA(DisplayName = "Arrow"),
	WP_SWORD UMETA(DisplayName = "Sword")
};

template<typename TEnum>
static FORCEINLINE FString GetEnumValueAsString(const FString& Name, TEnum Value) {
	const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
	if (!enumPtr) return FString("Invalid");
	return enumPtr->GetNameByValue((int64)Value).ToString();
}

/**
 * 
 */
class CROWSBOW_API CrowsBowEnumNames
{
public:
	CrowsBowEnumNames();
	~CrowsBowEnumNames();
};
