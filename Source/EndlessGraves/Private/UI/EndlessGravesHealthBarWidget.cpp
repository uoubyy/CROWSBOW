// Copyright EndlessGraves, Inc. All Rights Reserved.


#include "UI/EndlessGravesHealthBarWidget.h"
#include "Components/ProgressBar.h"

void UEndlessGravesHealthBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (HealthProgressBar != nullptr)
	{
		HealthProgressBar->PercentDelegate.BindUFunction(this, "GetHealthPercentage");
		HealthProgressBar->SynchronizeProperties();
	}
}

float UEndlessGravesHealthBarWidget::GetHealthPercentage()
{
	return HealthPercentage;
}

void UEndlessGravesHealthBarWidget::SetHealthPercentage(float value)
{
	HealthPercentage = value;
}