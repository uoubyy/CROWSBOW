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

	//virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {}

	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UPROPERTY(EditDefaultsOnly, Category = Damage)
	float MaxAttackDistance;

	UPROPERTY(EditDefaultsOnly, Category = Damage)
	TSubclassOf<class AEndlessGravesFireBall> FireBallClass;

private:

	UPROPERTY()
	class AEndlessGravesFireBall* FireBallActor = nullptr;

	bool DamageImmunity = false;

	UFUNCTION()
	void UnlockDamageImmunity() { DamageImmunity = false; }

	FTimerHandle DamageImmunityTimeHandler;
};
