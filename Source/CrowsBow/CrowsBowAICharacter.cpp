// Fill out your copyright notice in the Description page of Project Settings.


#include "CrowsBowAICharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"

#include "CrowsBoneFireBall.h"

#include "DrawDebugHelpers.h"

// Sets default values
ACrowsBowAICharacter::ACrowsBowAICharacter()
{
	BatEyesMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BatEyesMesh"));
	BatEyesMesh->AttachTo(GetMesh(), FName(TEXT("Face")), EAttachLocation::KeepRelativeOffset, false);

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	GetCharacterMovement()->GravityScale = 0;
}

// Called when the game starts or when spawned
void ACrowsBowAICharacter::BeginPlay()
{
	Super::BeginPlay();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &ACrowsBowAICharacter::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &ACrowsBowAICharacter::OnNoiseHeard);
	
}

void ACrowsBowAICharacter::OnPawnSeen(APawn* SeenPawn)
{
	if (SeenPawn == nullptr)
		return;

	//DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 12, FColor::Red, false, 10.0f);

	FVector batLocation = GetActorLocation();
	FVector seenPawnLoc = SeenPawn->GetActorLocation();

	TargetDirection = (seenPawnLoc - batLocation);
	if (TargetDirection.Size() <= MaxAttackDistance)
	{
		ShouldChase = false;
		GetWorldTimerManager().ClearTimer(ChasingTimeHandler);
		AttackPlayer(seenPawnLoc);
		return;
	}

	TargetDirection.Normalize();

	if (!ShouldChase)
	{
		ShouldChase = true;
		GetWorldTimerManager().SetTimer(ChasingTimeHandler, this, &ACrowsBowAICharacter::ChaseToSenseActor, 0.05f, true);
	}
}

// if seen must heard
void ACrowsBowAICharacter::OnNoiseHeard(APawn* HeardPawn, const FVector& Location, float Volume)
{
	//DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Yellow, false, 10.0f);

	FVector batLocation = GetActorLocation();

	TargetDirection = (Location - batLocation);
	TargetDirection.Normalize();

	if (!ShouldTurn)
	{
		ShouldTurn = true;
		GetWorldTimerManager().SetTimer(TurnningTimerHandle, this, &ACrowsBowAICharacter::TurnToSenseActor, 0.05f, true);
	}
}

void ACrowsBowAICharacter::TurnToSenseActor()
{
	SetActorRotation(FMath::Lerp(GetActorRotation(), TargetDirection.Rotation(), 0.05f));

	if (PawnSensingComp->CanSenseAnything() == false)
	{
		ShouldTurn = false;
		ShouldChase = false;
		GetWorldTimerManager().ClearTimer(TurnningTimerHandle);
		GetWorldTimerManager().ClearTimer(ChasingTimeHandler);
	}
}

void ACrowsBowAICharacter::ChaseToSenseActor()
{
	if (PawnSensingComp->CanSenseAnything() == false)
	{
		ShouldTurn = false;
	}
}

void ACrowsBowAICharacter::AttackPlayer(FVector tergetLoc)
{
	const FVector SocketLocation = GetMesh()->GetSocketLocation("Nose");
	FVector forwardDir = tergetLoc - GetActorLocation();
	forwardDir.Z = 0.0f;
	forwardDir.Normalize();

	FRotator spawnRotation = FRotationMatrix::MakeFromX(forwardDir).Rotator();

	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(0, 1.5f, FColor::Red, FString::Printf(TEXT("Forward direction %s"), *forwardDir.ToString()));

	UWorld* const World = GetWorld();

	//DrawDebugLine(World, SocketLocation, SocketLocation + forwardDir * 200.0f, FColor::Red, true, -1.0f, 0, 5.0f);

	if (FireBallActor == nullptr)
	{
		if (World != nullptr)
		{
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			FireBallActor = World->SpawnActor<ACrowsBoneFireBall>(FireBallClass, SocketLocation, spawnRotation, ActorSpawnParams);

			FireBallActor->ActiveFireBall(SocketLocation, spawnRotation, forwardDir);
		}
	}
	else
	{
		FireBallActor->ActiveFireBall(SocketLocation, spawnRotation, forwardDir);
	}
}
