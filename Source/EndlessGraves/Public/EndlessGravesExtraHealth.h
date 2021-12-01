// Copyright EndlessGraves, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EndlessGravesPowerUpInterface.h"
#include "EndlessGravesExtraHealth.generated.h"

UCLASS()
class ENDLESSGRAVES_API AEndlessGravesExtraHealth : public AActor, public IEndlessGravesPowerUpInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEndlessGravesExtraHealth();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = PowerUp)
	float AddExtraHealth = 50.0f;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, Category = PowerUp)
	class USphereComponent* SphereComponent;

	UPROPERTY(EditDefaultsOnly, Category = PowerUp)
	float SelfRotationSpeed = 100.0f;

public:	

	virtual void Tick(float DeltaSeconds) override;

	virtual EPowerUpType GetPowerUpType() const override { return EPowerUpType::POWERUP_ADDHEALTH; }

	virtual float GetPowerUpEffectValue() const override { return AddExtraHealth; }

protected:
	UFUNCTION(BlueprintCallable)
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
