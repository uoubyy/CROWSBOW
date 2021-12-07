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

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUserWidget> GameFailMenuClass;

	UPROPERTY(BlueprintReadWrite, Category = UI)
	class UUserWidget* GameFailMenuInstance;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUserWidget> GameSuccessMenuClass;

	UPROPERTY(BlueprintReadWrite, Category = UI)
	class UUserWidget* GameSuccessMenuInstance;

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

	UFUNCTION(BlueprintCallable)
	void OnStartGame();

	UFUNCTION(BlueprintCallable)
	void OnGameOver(bool result);

	UFUNCTION(BlueprintCallable, Category = "Twitch")
	void SummonEnemy(EEnemyType enemyType, int num);

	UFUNCTION(BlueprintCallable, Category = "Twitch")
	void SpawnExtraHealthPowerUp();

	UFUNCTION(BlueprintCallable, Category = "Debug")
	void SummonSkeleton() { SummonEnemy(EEnemyType::EET_Skeleton, 1); }

	UFUNCTION(BlueprintCallable, Category = "Debug")
	void SummonDragon() { SummonEnemy(EEnemyType::EET_Dragon, 1);}

	virtual void BeginPlay() override;
};
