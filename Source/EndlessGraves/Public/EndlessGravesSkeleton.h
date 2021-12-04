// Copyright EndlessGraves, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EndlessGravesAICharacter.h"
#include "EndlessGravesCustomEnum.h"
#include "EndlessGravesWeaponInterface.h"
#include "EndlessGravesSkeleton.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSGRAVES_API AEndlessGravesSkeleton : public AEndlessGravesAICharacter, public IEndlessGravesWeaponInterface
{
	GENERATED_BODY()

public:
	AEndlessGravesSkeleton();

	virtual float GetDamage() const override { return 10.0f; }

	virtual EDamageType GetDamageType() const override { return EDamageType::EDT_Constant; }
	
protected:

	virtual void BeginPlay() override;

	virtual void OnPawnSeen(APawn* SeenPawn) override;

	virtual void OnNoiseHeard(APawn* HeardPawn, const FVector& Location, float Volume) override;

	virtual void AttackPlayer(FVector PlayerLocation);

	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UPROPERTY(VisibleAnywhere, Category = Movement)
	float ChaseSpeed = 100.0f;

	virtual void ChangeStateInto(EEnemyState newState) override;

	virtual void GenerateNewState() override;

private:

	FVector TargetDirection;
};
