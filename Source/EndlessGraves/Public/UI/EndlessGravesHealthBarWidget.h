// Copyright EndlessGraves, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndlessGravesHealthBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSGRAVES_API UEndlessGravesHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* HealthProgressBar;

	UPROPERTY(VisibleAnywhere)
	float HealthPercentage = 1.0f;

public:
	UFUNCTION()
	void SetHealthPercentage(float value);

	UFUNCTION()
	float GetHealthPercentage();
};
