// Copyright EndlessGraves, Inc. All Rights Reserved.


#include "EndlessGravesDragon.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Perception/PawnSensingComponent.h"

#include "EndlessGravesFireBall.h"

#include "DrawDebugHelpers.h"

AEndlessGravesDragon::AEndlessGravesDragon() : AEndlessGravesAICharacter()
{
	CapsuleComp->BodyInstance.bLockXRotation = true;
	CapsuleComp->BodyInstance.bLockYRotation = true;
}

void AEndlessGravesDragon::BeginPlay()
{
	Super::BeginPlay();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &AEndlessGravesDragon::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AEndlessGravesDragon::OnNoiseHeard);
}

void AEndlessGravesDragon::OnPawnSeen(APawn* SeenPawn)
{
	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 12, FColor::Red, false, 10.0f);

	Super::OnPawnSeen(SeenPawn);

	float distance = (SensedLocation - GetActorLocation()).Size();

	if (distance <= MaxAttackDistance)
	{
		AttackPlayer(SensedLocation);
	}
}

void AEndlessGravesDragon::OnNoiseHeard(APawn* HeardPawn, const FVector& Location, float Volume)
{
	Super::OnNoiseHeard(HeardPawn, Location, Volume);

	DrawDebugSphere(GetWorld(), HeardPawn->GetActorLocation(), 32.0f, 12, FColor::Yellow, false, 10.0f);
}

void AEndlessGravesDragon::AttackPlayer(FVector PlayerLocation)
{
	FVector direction = PlayerLocation - GetActorLocation();
	direction.Normalize();

	if (FireBallActor == nullptr)
	{
		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			const FVector SocketLocation = SkeletalMeshComp->GetSocketLocation("Bone_002");
			FRotator spawnRotation = FRotationMatrix::MakeFromX(direction).Rotator();
			FireBallActor = World->SpawnActor<AEndlessGravesFireBall>(FireBallClass, SocketLocation, spawnRotation, ActorSpawnParams);
		}
	}

	FireBallActor->Active(direction);
}