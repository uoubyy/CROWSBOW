//  Copyright Yanyi Bao. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CrowsBowEnemyBCharacter.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	ES_Idle UMETA(DisplayName = "Idle"),
	ES_Running UMETA(DisplayName = "Running"),
	ES_Attack UMETA(DisplayName = "Attack")
};

UCLASS()
class CROWSBOW_API ACrowsBowEnemyBCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACrowsBowEnemyBCharacter();

	UFUNCTION(BlueprintCallable, Category=Enemy)
	EEnemyState GetEnemyState();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category=Enemy)
	EEnemyState CurEnemyState;

	UPROPERTY(VisibleAnywhere, Category = Components)
		class UPawnSensingComponent* PawnSensingComp;

	UFUNCTION()
		void OnPawnSeen(APawn* SeenPawn);

	UFUNCTION()
		void OnNoiseHeard(APawn* HeardPawn, const FVector& Location, float Volume);

	UPROPERTY(VisibleAnywhere)
		class UWidgetComponent* HealthWidgetComp;

	UPROPERTY(VisibleAnywhere, Category = Health)
		class UCrowsBowHealthBarWidget* HealthBar;

	UPROPERTY(EditDefaultsOnly, Category = Health)
		float MaxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, Category = Health)
		float CurHealth;

public:	

	UFUNCTION()
		void TurnToSenseActor();

	UFUNCTION()
		void ChaseToSenseActor();

	UFUNCTION()
		void AttackPlayer(FVector tergetLoc);

};