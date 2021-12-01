// Copyright EndlessGraves, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EndlessGravesProjectile.h"
#include "EndlessGravesWeaponInterface.h"
#include "EndlessGravesFireBall.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSGRAVES_API AEndlessGravesFireBall : public AEndlessGravesProjectile, public IEndlessGravesWeaponInterface
{
	GENERATED_BODY()
	
public:
	AEndlessGravesFireBall();

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere)
	class UParticleSystemComponent* ParticleSystemComponent;

	UFUNCTION(BlueprintCallable, Category = FireBall)
	void Active(FVector location, FVector direction);

	UFUNCTION(BlueprintCallable, Category = FireBall)
	void DeActive();

	UPROPERTY(EditDefaultsOnly, Category = Damage)
	float Damage;

	virtual float GetDamage() const override { return Damage; };

	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

protected:
	virtual void BeginPlay() override;

	FTimerHandle DeActiveTimerHandle;
};
