// Fill out your copyright notice in the Description page of Project Settings.


#include "CrowsBowAICharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"

#include "CrowsBowFireBall.h"
#include <Components/WidgetComponent.h>
#include "CrowsBowHealthBarWidget.h"
#include "CrowsBowProjectile.h"

#include "DrawDebugHelpers.h"

// Sets default values
ACrowsBowAICharacter::ACrowsBowAICharacter()
{
	BatEyesMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BatEyesMesh"));
	BatEyesMesh->AttachTo(GetMesh(), FName(TEXT("Face")), EAttachLocation::KeepRelativeOffset, false);

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	GetCharacterMovement()->GravityScale = 0;

	HealthWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthWidgetComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	CurHealth = MaxHealth;
}

// Called when the game starts or when spawned
void ACrowsBowAICharacter::BeginPlay()
{
	Super::BeginPlay();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &ACrowsBowAICharacter::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &ACrowsBowAICharacter::OnNoiseHeard);

	HealthBar = Cast<UCrowsBowHealthBarWidget>(HealthWidgetComp->GetUserWidgetObject());
	HealthBar->UpdateHealthPercentage(1.0f);
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
		GetWorldTimerManager().SetTimer(TurningTimerHandle, this, &ACrowsBowAICharacter::TurnToSenseActor, 0.05f, true);
	}
}

void ACrowsBowAICharacter::TurnToSenseActor()
{
	SetActorRotation(FMath::Lerp(GetActorRotation(), TargetDirection.Rotation(), 0.05f));

	if (PawnSensingComp->CanSenseAnything() == false)
	{
		ShouldTurn = false;
		ShouldChase = false;
		GetWorldTimerManager().ClearTimer(TurningTimerHandle);
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
	const FVector SocketLocation = GetMesh()->GetSocketLocation("Bone_002");
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

			FireBallActor = World->SpawnActor<ACrowsBowFireBall>(FireBallClass, SocketLocation, spawnRotation, ActorSpawnParams);

			FireBallActor->ActiveFireBall(SocketLocation, spawnRotation, forwardDir);
		}
	}
	else
	{
		FireBallActor->ActiveFireBall(SocketLocation, spawnRotation, forwardDir);
	}
}

void ACrowsBowAICharacter::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red, "ACrowsBowAICharacter hitted");

	ACrowsBowProjectile* arrow = Cast<ACrowsBowProjectile>(OtherActor);
	if (arrow)
	{
		CurHealth -= arrow->DamageValue;
	}

	HealthBar->UpdateHealthPercentage(CurHealth / MaxHealth);
}
