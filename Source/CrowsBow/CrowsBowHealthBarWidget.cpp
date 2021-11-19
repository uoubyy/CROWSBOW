//  Copyright Yanyi Bao. All Rights Reserved.

#include "CrowsBowHealthBarWidget.h"
#include "Components/ProgressBar.h"

void UCrowsBowHealthBarWidget::NativeConstruct() 
{
  Super::NativeConstruct();

  if (HealthProgressBar != nullptr) 
  {
      HealthProgressBar->PercentDelegate.BindUFunction(this, "GetHealthPercentage");
      HealthProgressBar->SynchronizeProperties();
  }
}

float UCrowsBowHealthBarWidget::GetHealthPercentage()
{
   return HealthPercentage;
}

void UCrowsBowHealthBarWidget::UpdateHealthPercentage(float value)
{
    HealthPercentage = value;
}