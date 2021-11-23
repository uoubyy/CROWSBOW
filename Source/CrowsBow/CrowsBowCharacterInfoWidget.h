//  Copyright Yanyi Bao. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CrowsBowEnumNames.h"
#include "CrowsBowCharacterInfoWidget.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class CROWSBOW_API UCrowsBowCharacterInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateLifeStatus(int currentLife, float percent);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateArrowCD(int leftArrowNum);

	UFUNCTION(BlueprintImplementableEvent)
	void SwitchWeapon(WeaponType weaponType);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Avatar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* WeaponSlot1;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* WeaponSlot2;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Life1;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Life2;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Life3;

	UPROPERTY(BlueprintReadOnly)
	int LeftLife = 3;

	virtual void NativeConstruct() override;
	
};
