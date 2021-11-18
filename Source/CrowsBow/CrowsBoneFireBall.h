//  Copyright Yanyi Bao. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CrowsBoneFireBall.generated.h"

class UStaticMeshComponent;
class APlayerCameraManager;
class UProjectileMovementComponent;

UCLASS()
class CROWSBOW_API ACrowsBoneFireBall : public AActor
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	ACrowsBoneFireBall();

	UFUNCTION(BlueprintImplementableEvent, Category= FireBall)
	void FaceToPlayerCamera();

	UFUNCTION()
	void ActiveFireBall(FVector location, FRotator rotation, FVector direction);

	UFUNCTION()
	void DeActiveFireBall();

	UPROPERTY(EditAnywhere, Category=FireBall)
	float ImpulseForce = 1.0f;

protected:
	FTimerHandle ActiveTimerHandle;
	FTimerHandle DeActiveTimerHandle;

	virtual void BeginPlay() override;

	UPROPERTY()
	UProjectileMovementComponent* ProjectileComp;
};
