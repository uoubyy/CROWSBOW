// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CrowsBowAICharacter.generated.h"

class UPawnSensingComponent;
class ACrowsBowCharacter;
class ACrowsBowFireBall;

UCLASS()
class CROWSBOW_API ACrowsBowAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = Ability)
	TSubclassOf<class ACrowsBowFireBall> FireBallClass;

	UPROPERTY(EditDefaultsOnly, Category= Ability)
	float MaxAttackDistance = 500.0f;// fire ball attack area

	// Sets default values for this character's properties
	ACrowsBowAICharacter();

	UFUNCTION(BlueprintCallable)
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = BatEyes)
	class USkeletalMeshComponent* BatEyesMesh;

	UPROPERTY(VisibleAnywhere, Category = Components)
	UPawnSensingComponent* PawnSensingComp;

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

private:
	bool ShouldTurn = false;
	bool ShouldChase = false;

	FTimerHandle TurningTimerHandle;
	FTimerHandle ChasingTimeHandler;
	FTimerHandle AttackTimeHandler;

	FVector TargetDirection;

	UPROPERTY()
	ACrowsBowFireBall* FireBallActor = nullptr;

	UFUNCTION()
	void TurnToSenseActor();

	UFUNCTION()
	void ChaseToSenseActor();

	UFUNCTION()
	void AttackPlayer(FVector tergetLoc);
};
