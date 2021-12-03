// Copyright EndlessGraves, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EndlessGravesCustomEnum.h"
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

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUserWidget> PauseMenuClass;

	UPROPERTY(BlueprintReadWrite, Category = UI)
	class UUserWidget* PauseMenuInstance;

	UPROPERTY(EditDefaultsOnly, Category = "Twitch")
	TSubclassOf<class AEndlessGravesExtraHealth> ExtraHealthClass;

	UPROPERTY(VisibleAnywhere, Category = "Twitch")
	TArray<class AActor*> AllTombs;

public:
	UFUNCTION(BlueprintCallable)
	void OnPauseGame();

	UFUNCTION(BlueprintCallable)
	void OnResumeGame();

	UFUNCTION(BlueprintCallable)
	void OnRestartGame();

	UFUNCTION(BlueprintCallable, Category = "Twitch")
	void SummonEnemy(EEnemyType enemyType, int num);

	UFUNCTION(BlueprintCallable, Category = "Twitch")
	void SpawnExtraHealthPowerUp();

	virtual void BeginPlay() override;
};
