//  Copyright Yanyi Bao. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CrowsBowHealthBarWidget.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class CROWSBOW_API UCrowsBowHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UProgressBar* HealthProgressBar;

	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere)
		float HealthPercentage = 1.0f;

	UFUNCTION()
		float GetHealthPercentage();

public:
	UFUNCTION()
	void UpdateHealthPercentage(float value);
};
