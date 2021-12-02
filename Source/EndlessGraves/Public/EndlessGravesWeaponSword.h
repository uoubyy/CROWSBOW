// Copyright EndlessGraves, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EndlessGravesWeaponInterface.h"
#include "EndlessGravesWeaponSword.generated.h"


UCLASS()
class ENDLESSGRAVES_API AEndlessGravesWeaponSword : public AActor, public IEndlessGravesWeaponInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEndlessGravesWeaponSword();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* MeshComponent;

public:	

	virtual float GetDamage() const override;

	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
