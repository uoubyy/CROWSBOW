// Copyright EndlessGraves, Inc. All Rights Reserved.


#include "EndlessGravesPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "EndlessGravesExtraHealth.h"
#include "EndlessGravesTomb.h"

#include "Math/UnrealMathUtility.h"

void AEndlessGravesPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Pause", IE_Pressed, this, &AEndlessGravesPlayerController::OnPauseGame);
	InputComponent->BindAction("PowerUpHealth", IE_Pressed, this, &AEndlessGravesPlayerController::SpawnExtraHealthPowerUp);

	InputComponent->BindAction("SummonDragon", IE_Pressed, this, &AEndlessGravesPlayerController::SummonDragon);
	InputComponent->BindAction("SummonSkeleton", IE_Pressed, this, &AEndlessGravesPlayerController::SummonSkeleton);
}

void AEndlessGravesPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEndlessGravesTomb::StaticClass(), AllTombs);
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

void AEndlessGravesPlayerController::OnStartGame()
{
	
}

void AEndlessGravesPlayerController::SpawnExtraHealthPowerUp()
{
	FVector CharacterLocation = GetPawn()->GetActorLocation();
	FVector ForwardDirection = GetPawn()->GetActorForwardVector();
	FVector PowerUpSpawnLocation = CharacterLocation + ForwardDirection * 200.0f;
	PowerUpSpawnLocation.Z += 50.0f;

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;// ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		AEndlessGravesExtraHealth* HealthPowerUp = World->SpawnActor<AEndlessGravesExtraHealth>(ExtraHealthClass, PowerUpSpawnLocation, FRotator::ZeroRotator, ActorSpawnParams);
	}

	SummonEnemy(EEnemyType::EET_Skeleton, 3);
}

void AEndlessGravesPlayerController::SummonEnemy(EEnemyType enemyType, int num)
{
	if (AllTombs.Num() > 0)
	{
		int32 LastIndex = AllTombs.Num() - 1;
		for (int32 i = 0; i <= LastIndex; ++i)
		{
			int32 Index = FMath::RandRange(i, LastIndex);
			if (i != Index)
			{
				AllTombs.Swap(i, Index);
			}
		}
	}

	FVector CharacterLocation = GetPawn()->GetActorLocation();

	for (int i = 0; i < num && i < AllTombs.Num(); i++)
	{
		AEndlessGravesTomb* tomb = Cast<AEndlessGravesTomb>(AllTombs[i]);
		if (tomb)
		{ 
			tomb->SummonEnemy(enemyType, CharacterLocation);
		}
	}
}


void AEndlessGravesPlayerController::OnGameOver(bool result)
{
	if(result == false)
	{ 
		if (GameFailMenuInstance && GameFailMenuInstance->IsInViewport())
			return;

		GameFailMenuInstance = GameFailMenuInstance == nullptr ? CreateWidget<UUserWidget>(this, GameFailMenuClass) : GameFailMenuInstance;
		if (GameFailMenuInstance)
		{
			GameFailMenuInstance->AddToViewport(100);

			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());

			// Single-player only
			if (GetWorld()->IsNetMode(NM_Standalone))
			{
				UGameplayStatics::SetGamePaused(this, true);
			}
		}
	}
	else
	{
		if (GameSuccessMenuInstance && GameSuccessMenuInstance->IsInViewport())
			return;

		GameSuccessMenuInstance = GameSuccessMenuInstance == nullptr ? CreateWidget<UUserWidget>(this, GameSuccessMenuClass) : GameSuccessMenuInstance;
		if (GameSuccessMenuInstance)
		{
			GameSuccessMenuInstance->AddToViewport(100);

			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());

			// Single-player only
			if (GetWorld()->IsNetMode(NM_Standalone))
			{
				UGameplayStatics::SetGamePaused(this, true);
			}
		}
	}
}