//  Copyright Yanyi Bao. All Rights Reserved.


#include "CrowsBowFireBall.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ACrowsBowFireBall::ACrowsBowFireBall()
{
}

void ACrowsBowFireBall::BeginPlay()
{
	Super::BeginPlay();

	ProjectileComp = FindComponentByClass<UProjectileMovementComponent>();
}

void ACrowsBowFireBall::ActiveFireBall(FVector location, FRotator rotation, FVector direction)
{
	SetActorHiddenInGame(false);
	
	ProjectileComp->Velocity = direction * 1000.0f;
	ProjectileComp->Activate(true);
	SetActorLocationAndRotation(location, rotation);

	GetWorldTimerManager().SetTimer(DeActiveTimerHandle, this, &ACrowsBowFireBall::DeActiveFireBall, 1.0f, false);
}

void ACrowsBowFireBall::DeActiveFireBall()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red, "DeActive Fire Ball.");

	SetActorHiddenInGame(true);
	ProjectileComp->Activate(false);
}