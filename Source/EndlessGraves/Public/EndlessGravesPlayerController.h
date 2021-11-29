// Copyright EndlessGraves, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EndlessGravesPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSGRAVES_API AEndlessGravesPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void SetupInputComponent() override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PauseMenuClass;

	UPROPERTY(BlueprintReadWrite, Category = UI)
	class UUserWidget* PauseMenuInstance;

private:
	UFUNCTION(BlueprintCallable)
	void OnPauseGame();

	UFUNCTION(BlueprintCallable)
	void OnResumeGame();

	UFUNCTION(BlueprintCallable)
	void OnRestartGame();
};
