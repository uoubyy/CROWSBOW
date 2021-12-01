// Copyright EndlessGraves, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EndlessGravesProjectile.h"
#include "EndlessGravesWeaponInterface.h"
#include "EndlessGravesWeaponArrow.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSGRAVES_API AEndlessGravesWeaponArrow : public AEndlessGravesProjectile, public IEndlessGravesWeaponInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = Mesh)
	class UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(VisibleAnywhere, Category = Collision)
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(EditDefaultsOnly, Category = Damage)
	float Damage;

public:
	AEndlessGravesWeaponArrow();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual float GetDamage() const override { return Damage; };
};
