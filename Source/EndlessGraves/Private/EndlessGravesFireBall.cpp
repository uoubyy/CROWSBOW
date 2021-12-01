// Copyright EndlessGraves, Inc. All Rights Reserved.


#include "EndlessGravesFireBall.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AEndlessGravesFireBall::AEndlessGravesFireBall()
{
	// SphereComponent initialization
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->InitSphereRadius(50.0f);
	SphereComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	SphereComponent->AttachToComponent(RootScene, FAttachmentTransformRules::KeepRelativeTransform);
	SphereComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	// ParticleSystemComponent initialization
	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	ParticleSystemComponent->SetAutoActivate(false);
	ParticleSystemComponent->SetupAttachment(RootScene);
	ParticleSystemComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
}

void AEndlessGravesFireBall::BeginPlay()
{
	Super::BeginPlay();
	SetActorHiddenInGame(true);

	SphereComponent->OnComponentHit.AddDynamic(this, &AEndlessGravesFireBall::OnHit);
}

void AEndlessGravesFireBall::Active(FVector location, FVector direction)
{
	SetActorLocation(location);
	SetActorHiddenInGame(false);
	ParticleSystemComponent->SetActive(true);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	UProjectileMovementComponent* projectileComp = GetProjectileMovement();
	GetProjectileMovement()->Velocity = direction * 1000.0f;
	projectileComp->Activate(true);

	GetWorldTimerManager().SetTimer(DeActiveTimerHandle, this, &AEndlessGravesFireBall::DeActive, 1.0f, false);
}

void AEndlessGravesFireBall::DeActive()
{
	SetActorHiddenInGame(true);
	ParticleSystemComponent->SetActive(false);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEndlessGravesFireBall::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	DeActive();
}