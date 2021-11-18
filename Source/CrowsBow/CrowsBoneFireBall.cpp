//  Copyright Yanyi Bao. All Rights Reserved.


#include "CrowsBoneFireBall.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ACrowsBoneFireBall::ACrowsBoneFireBall()
{
}

void ACrowsBoneFireBall::BeginPlay()
{
	Super::BeginPlay();

	ProjectileComp = FindComponentByClass<UProjectileMovementComponent>();
}

void ACrowsBoneFireBall::ActiveFireBall(FVector location, FRotator rotation, FVector direction)
{
	SetActorHiddenInGame(false);
	
	ProjectileComp->Velocity = direction * 1000.0f;
	ProjectileComp->Activate(true);
	SetActorLocationAndRotation(location, rotation);

	GetWorldTimerManager().SetTimer(DeActiveTimerHandle, this, &ACrowsBoneFireBall::DeActiveFireBall, 1.0f, false);
}

void ACrowsBoneFireBall::DeActiveFireBall()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red, "DeActive Fire Ball.");

	SetActorHiddenInGame(true);
	ProjectileComp->Activate(false);
}