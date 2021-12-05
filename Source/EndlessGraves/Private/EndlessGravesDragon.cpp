// Copyright EndlessGraves, Inc. All Rights Reserved.


#include "EndlessGravesDragon.h"
#include "Components/CapsuleComponent.h"
#include "Perception/PawnSensingComponent.h"

#include "EndlessGravesFireBall.h"

#include "DrawDebugHelpers.h"

AEndlessGravesDragon::AEndlessGravesDragon() : AEndlessGravesAICharacter()
{
	//CapsuleComp->BodyInstance.bLockXRotation = true;
	//CapsuleComp->BodyInstance.bLockYRotation = true;
}

void AEndlessGravesDragon::BeginPlay()
{
	Super::BeginPlay();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &AEndlessGravesDragon::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AEndlessGravesDragon::OnNoiseHeard);

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEndlessGravesDragon::OnBeginOverlap);
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

	if (GetWorldTimerManager().IsTimerActive(TurningTimerHandle) == false)
		GetWorldTimerManager().SetTimer(TurningTimerHandle, this, &AEndlessGravesDragon::TurnToSenseActor, 0.05f, true);

	// DrawDebugSphere(GetWorld(), HeardPawn->GetActorLocation(), 32.0f, 12, FColor::Yellow, false, 10.0f);
}

void AEndlessGravesDragon::AttackPlayer(FVector PlayerLocation)
{
	Super::AttackPlayer(PlayerLocation);
	ensure(FireBallClass != nullptr);
	FVector direction = PlayerLocation - GetActorLocation();
	const FVector SocketLocation = GetMesh()->GetSocketLocation("Bone_002");
	direction.Normalize();

	if (FireBallActor == nullptr)
	{
		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			
			FRotator spawnRotation = FRotationMatrix::MakeFromX(direction).Rotator();
			FireBallActor = World->SpawnActor<AEndlessGravesFireBall>(FireBallClass, SocketLocation, spawnRotation, ActorSpawnParams);
		}
	}

	FireBallActor->Active(SocketLocation, direction);
}

void AEndlessGravesDragon::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("EndlessGravesDragon::OnBeginOverlap %s"), *(OtherActor->GetName())));

	IEndlessGravesWeaponInterface* Weapon = Cast<IEndlessGravesWeaponInterface>(OtherActor);
	if (Weapon && DamageImmunity == false)
	{ 
		CurHealth -= Weapon->GetDamage();
		GetWorldTimerManager().SetTimer(DamageImmunityTimeHandler, this, &AEndlessGravesDragon::UnlockDamageImmunity, 0.5f, false);
	}

	UpdateAIHUD();
}