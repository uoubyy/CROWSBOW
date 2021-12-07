// Copyright EndlessGraves, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EndlessGravesPickable.generated.h"

UCLASS()
class ENDLESSGRAVES_API AEndlessGravesPickable : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AEndlessGravesPickable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = Mesh)
	class UStaticMeshComponent* StaticMeshComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
