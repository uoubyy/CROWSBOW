// Copyright EndlessGraves, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EndlessGravesProjectile.h"
#include "EndlessGravesFireBall.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSGRAVES_API AEndlessGravesFireBall : public AEndlessGravesProjectile
{
	GENERATED_BODY()
	
public:
	AEndlessGravesFireBall();

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere)
	class UParticleSystemComponent* ParticleSystemComponent;

protected:
	virtual void BeginPlay() override;
};
