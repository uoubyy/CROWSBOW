// Copyright EndlessGraves, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EndlessGravesAICharacter.generated.h"

UCLASS()
class ENDLESSGRAVES_API AEndlessGravesAICharacter : public AActor
{
	GENERATED_BODY()

	class UEndlessGravesHealthBarWidget* HealthBar;
	
public:	
	// Sets default values for this actor's properties
	AEndlessGravesAICharacter();

	const float GetDamage() { return Damage; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	virtual void OnPawnSeen(APawn* SeenPawn);

	UFUNCTION(BlueprintCallable)
	virtual void OnNoiseHeard(APawn* HeardPawn, const FVector& Location, float Volume);

	UFUNCTION(BlueprintCallable)
	virtual void OnPawnLost();

	UFUNCTION()
	virtual void AttackPlayer(FVector PlayerLocation);

	UPROPERTY(VisibleAnywhere, Category = Components)
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere, Category = Components)
	class USkeletalMeshComponent* SkeletalMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UWidgetComponent* HealthWidgetComp;

	UPROPERTY(VisibleAnywhere, Category = Components)
	class UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(EditDefaultsOnly, Category = Damage)
	float Damage;

	UPROPERTY(EditDefaultsOnly, Category = Health)
	float MaxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, Category = Health)
	float CurHealth;

	FVector SensedLocation = FVector::ZeroVector;

private:

	FTimerHandle TurningTimerHandle;

	UFUNCTION()
	void TurnToSenseActor();
};
