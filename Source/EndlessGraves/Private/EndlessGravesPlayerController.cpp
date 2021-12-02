// Copyright EndlessGraves, Inc. All Rights Reserved.


#include "EndlessGravesPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "EndlessGravesExtraHealth.h"

void AEndlessGravesPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Pause", IE_Pressed, this, &AEndlessGravesPlayerController::OnPauseGame);
	InputComponent->BindAction("PowerUpHealth", IE_Pressed, this, &AEndlessGravesPlayerController::SpawnExtraHealthPowerUp);
	// InputComponent->BindAction("SummonEnemy", IE_Pressed, this, &AEndlessGravesPlayerController::SummonEnemy);
}

void AEndlessGravesPlayerController::OnPauseGame()
{
	if (PauseMenuInstance && PauseMenuInstance->IsInViewport())
		return;

	PauseMenuInstance = PauseMenuInstance == nullptr ? CreateWidget<UUserWidget>(this, PauseMenuClass) : PauseMenuInstance;
	if (PauseMenuInstance)
	{
		PauseMenuInstance->AddToViewport(100);

		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());

		// Single-player only
		if (GetWorld()->IsNetMode(NM_Standalone))
		{
			UGameplayStatics::SetGamePaused(this, true);
		}
	}
}

void AEndlessGravesPlayerController::OnResumeGame()
{
	if (PauseMenuInstance && PauseMenuInstance->IsInViewport())
	{
		PauseMenuInstance->RemoveFromParent();
		PauseMenuInstance = nullptr;

		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());

		// Single-player only
		if (GetWorld()->IsNetMode(NM_Standalone))
		{
			UGameplayStatics::SetGamePaused(this, false);
		}
	}
}

void AEndlessGravesPlayerController::OnRestartGame()
{

}

void AEndlessGravesPlayerController::SpawnExtraHealthPowerUp()
{
	FVector CharacterLocation = GetPawn()->GetActorLocation();
	FVector ForwardDirection = GetPawn()->GetActorForwardVector();
	FVector PowerUpSpawnLocation = CharacterLocation + ForwardDirection * 200.0f;
	PowerUpSpawnLocation.Z += 100.0f;

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;// ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		AEndlessGravesExtraHealth* HealthPowerUp = World->SpawnActor<AEndlessGravesExtraHealth>(ExtraHealthClass, PowerUpSpawnLocation, FRotator::ZeroRotator, ActorSpawnParams);
	}
}

void AEndlessGravesPlayerController::SummonEnemy(EEnemyType enemyType, int num)
{

}