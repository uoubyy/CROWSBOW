// Copyright EndlessGraves, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EndlessGravesCustomEnum.h"
#include "EndlessGravesTomb.generated.h"

UCLASS()
class ENDLESSGRAVES_API AEndlessGravesTomb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEndlessGravesTomb();

	UPROPERTY(EditDefaultsOnly, Category = SpawnEnemy)
	TSubclassOf<class AEndlessGravesSkeleton> EnemySkeletonClass;

	UPROPERTY(EditDefaultsOnly, Category = SpawnEnemy)
	TSubclassOf<class AEndlessGravesDragon> EnemyDragonClass;

	UPROPERTY(VisibleAnywhere, Category = Mesh)
	class USceneComponent* RootScene;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* EnemySpawnLocation;

	UPROPERTY(VisibleAnywhere, Category = Mesh)
	class UStaticMeshComponent* StaticMeshComp;

	UFUNCTION(BlueprintCallable, Category = SpawnEnemy)
	void SummonEnemy(EEnemyType enemyType);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
};
