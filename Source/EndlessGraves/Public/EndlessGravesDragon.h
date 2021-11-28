// Copyright EndlessGraves, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EndlessGravesAICharacter.h"
#include "EndlessGravesDragon.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSGRAVES_API AEndlessGravesDragon : public AEndlessGravesAICharacter
{
	GENERATED_BODY()

public:
	AEndlessGravesDragon();
	
protected:
	virtual void BeginPlay() override;

	// UFUNCTION(BlueprintCallable)
	virtual void OnPawnSeen(APawn* SeenPawn) override;

	// UFUNCTION(BlueprintCallable)
	virtual void OnNoiseHeard(APawn* HeardPawn, const FVector& Location, float Volume) override;

	// UFUNCTION()
	virtual void AttackPlayer(FVector PlayerLocation);

	UPROPERTY(EditDefaultsOnly, Category = Damage)
	float MaxAttackDistance;

	UPROPERTY(EditDefaultsOnly, Category = Damage)
	TSubclassOf<class AEndlessGravesFireBall> FireBallClass;

private:

	UPROPERTY()
	class AEndlessGravesFireBall* FireBallActor = nullptr;
};
