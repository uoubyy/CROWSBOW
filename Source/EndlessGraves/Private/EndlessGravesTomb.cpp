// Copyright EndlessGraves, Inc. All Rights Reserved.


#include "EndlessGravesTomb.h"
#include "EndlessGravesSkeleton.h"
#include "EndlessGravesDragon.h"
#include "EndlessGravesAIController.h"

#include "Components/StaticMeshComponent.h"

// Sets default values
AEndlessGravesTomb::AEndlessGravesTomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = RootScene;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(RootComponent);

	EnemySpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("EnemySpawnLocation"));
	EnemySpawnLocation->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEndlessGravesTomb::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEndlessGravesTomb::SummonEnemy(EEnemyType enemyType, FVector playerLocation)
{
	ensure(EnemyDragonClass && EnemySkeletonClass && TEXT("Dragon or Skeleton class is NULL!"));
	UWorld* const World = GetWorld();
	if(World)
	{ 
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		const FVector SpawnLocation = EnemySpawnLocation->GetComponentLocation();

		switch (enemyType)
		{
		case EEnemyType::EET_Skeleton:
		{
			// AEndlessGravesSkeleton* skeleton = 
			World->SpawnActor<AEndlessGravesSkeleton>(EnemySkeletonClass, SpawnLocation + FVector(0.0f, 0.0f, 50.0f),  FRotator::ZeroRotator, SpawnInfo);
			break;
		}
		case EEnemyType::EET_Dragon:
		{
			// AEndlessGravesDragon* dragon = 
			World->SpawnActor<AEndlessGravesDragon>(EnemyDragonClass, SpawnLocation + FVector(0.0f, 0.0f, 300.0f), FRotator::ZeroRotator, SpawnInfo);
			break;
		}
		default:
			break;
		}
	}
}