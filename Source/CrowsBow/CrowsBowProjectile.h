//  Copyright Yanyi Bao. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "CrowsBowEnumName.h"
#include "CrowsBowProjectile.generated.h"

class UCapsuleComponent;
class UProjectileMovementComponent;
class UStaticMeshComponent;

UCLASS()
class CROWSBOW_API ACrowsBowProjectile : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = Mesh)
	UStaticMeshComponent* MeshComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FArrowHitDelegate, FString, _ActorName, float, _DamgeValue);

public:
	UPROPERTY(EditAnywhere)
	float DamageValue;

	UPROPERTY(VisibleAnywhere, Category = Projectile)
	UCapsuleComponent* CollisionComp;

	// Sets default values for this actor's properties
	ACrowsBowProjectile();

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Returns CollisionComp subobject **/
	UCapsuleComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	UPROPERTY(BlueprintAssignable)
	FArrowHitDelegate OnArrowHitEvent;

protected:
	virtual void BeginPlay() override;
};
