//  Copyright Yanyi Bao. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CrowsBowFireBall.generated.h"

class UStaticMeshComponent;
class APlayerCameraManager;
class UProjectileMovementComponent;

UCLASS()
class CROWSBOW_API ACrowsBowFireBall : public AActor
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	ACrowsBowFireBall();

	UFUNCTION(BlueprintImplementableEvent, Category= FireBall)
	void ActiveParticle(bool state);

	UFUNCTION()
	void ActiveFireBall(FVector location, FRotator rotation, FVector direction);

	UFUNCTION()
	void DeActiveFireBall();

	UPROPERTY(EditAnywhere, Category=FireBall)
	float ImpulseForce = 1.0f;

	UPROPERTY(VisibleAnywhere, Category = Damage)
	float DamageValue = 50.0f;

protected:
	UPROPERTY()
	FTimerHandle ActiveTimerHandle;

	UPROPERTY()
	FTimerHandle DeActiveTimerHandle;

	virtual void BeginPlay() override;

	UPROPERTY()
	UProjectileMovementComponent* ProjectileComp;
};
