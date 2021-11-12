// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CrowsBowAICharacter.generated.h"

class UPawnSensingComponent;

UCLASS()
class CROWSBOW_API ACrowsBowAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACrowsBowAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="BatEyes")
	class USkeletalMeshComponent* BatEyesMesh;

	UPROPERTY(VisibleAnywhere, Category="Components")
	UPawnSensingComponent* PawnSensingComp;

	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);

	UFUNCTION()
	void OnNoiseHeard(APawn* HeardPawn, const FVector& Location, float Volume);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
