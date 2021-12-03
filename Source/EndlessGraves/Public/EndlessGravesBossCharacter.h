// Copyright EndlessGraves, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EndlessGravesAICharacter.h"
#include "EndlessGravesBossCharacter.generated.h"

UCLASS()
class ENDLESSGRAVES_API AEndlessGravesBossCharacter : public AEndlessGravesAICharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEndlessGravesBossCharacter();

protected:

	virtual void Tick( float DeltaSeconds ) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnPawnSeen(APawn* SeenPawn) override;

	virtual void OnNoiseHeard(APawn* HeardPawn, const FVector& Location, float Volume) override;

	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void TurnToSenseActor() override;

public:
	UFUNCTION(BlueprintCallable, Category = Boss)
	float GetRotationZ() {return 5.0f;}

	// random from 3.0 to 5.0
	float StateDuration;

	float CurStateTime;

	void GenerateNewState();
};
