// Copyright EndlessGraves, Inc. All Rights Reserved.


#include "EndlessGravesExtraHealth.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Math/Vector.h"

// Sets default values
AEndlessGravesExtraHealth::AEndlessGravesExtraHealth()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->InitSphereRadius(30.0f);
	SphereComponent->AttachToComponent(RootScene, FAttachmentTransformRules::KeepRelativeTransform);
	SphereComponent->SetRelativeLocation(FVector::ZeroVector);
}

// Called when the game starts or when spawned
void AEndlessGravesExtraHealth::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AEndlessGravesExtraHealth::OnBeginOverlap);
	
}

void AEndlessGravesExtraHealth::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FQuat QuatRotation = FQuat(FRotator(0, 1 * SelfRotationSpeed * DeltaSeconds, 0));
	AddActorLocalRotation(QuatRotation);
}

void AEndlessGravesExtraHealth::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// TODO OBJECT POOL
	Destroy();
}

