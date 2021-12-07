// Copyright EndlessGraves, Inc. All Rights Reserved.


#include "EndlessGravesPickable.h"

// Sets default values
AEndlessGravesPickable::AEndlessGravesPickable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	RootComponent = StaticMeshComponent;
}

// Called when the game starts or when spawned
void AEndlessGravesPickable::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEndlessGravesPickable::Tick(float DeltaTime)
{

}