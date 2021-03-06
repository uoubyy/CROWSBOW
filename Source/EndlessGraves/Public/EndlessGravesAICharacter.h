// Copyright EndlessGraves, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EndlessGravesCustomEnum.h"
#include "EndlessGravesAICharacter.generated.h"

UCLASS()
class ENDLESSGRAVES_API AEndlessGravesAICharacter : public ACharacter
{
	GENERATED_BODY()

	class UEndlessGravesHealthBarWidget* HealthBar;
	
public:	
	// Sets default values for this actor's properties
	AEndlessGravesAICharacter();

	const float GetDamage() { return Damage; }

	UFUNCTION(BlueprintCallable, Category = Enemy)
	EEnemyState GetEnemyState() const { return CurEnemyState; }

	UFUNCTION(BlueprintCallable, Category = Enemy)
	virtual void MoveToLocation(FVector Location);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAttackSound();

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

	UFUNCTION()
	virtual void UpdateAIHUD();

	UFUNCTION(BlueprintCallable)
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {}

	UFUNCTION(BlueprintCallable)
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {}

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UWidgetComponent* HealthWidgetComp;

	UPROPERTY(VisibleAnywhere, Category = Components)
	class UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(EditDefaultsOnly, Category = Damage)
	float Damage;

	UPROPERTY(EditDefaultsOnly, Category = Health)
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, Category = Health)
	float CurHealth;

	UPROPERTY(EditDefaultsOnly, Category = Ability)
	float MaxAttackDistance = 150.0f;

	FVector SensedLocation = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, Category = Enemy)
	EEnemyState CurEnemyState;

	FTimerHandle TurningTimerHandle;

	UFUNCTION()
	virtual void TurnToSenseActor();

	float StateDuration; // random number

	float CurStateTime;

	FTimerHandle ChangeStateHandle;

	virtual void GenerateNewState() {}

	FTimerHandle ChangeEnemyStateHandle;

	virtual void ChangeStateInto(EEnemyState newState) { CurEnemyState = newState; }

	bool DamageImmunity = false;

	UFUNCTION()
	void UnlockDamageImmunity() { DamageImmunity = false; }

	FTimerHandle DamageImmunityTimeHandler;
};
