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

	virtual void OnPawnSeen(APawn* SeenPawn) override;

	virtual void OnNoiseHeard(APawn* HeardPawn, const FVector& Location, float Volume) override;

	virtual void AttackPlayer(FVector PlayerLocation);

	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UPROPERTY(EditDefaultsOnly, Category = Damage)
	TSubclassOf<class AEndlessGravesFireBall> FireBallClass;

private:

	UPROPERTY()
	class AEndlessGravesFireBall* FireBallActor = nullptr;
};
