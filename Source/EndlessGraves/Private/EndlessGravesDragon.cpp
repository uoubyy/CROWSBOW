// Copyright EndlessGraves, Inc. All Rights Reserved.


#include "EndlessGravesDragon.h"
#include "Components/CapsuleComponent.h"
#include "Perception/PawnSensingComponent.h"

#include "EndlessGravesFireBall.h"

#include "DrawDebugHelpers.h"

TArray<class AEndlessGravesFireBall*> AEndlessGravesDragon::FireBallList; // need to recode object pool

AEndlessGravesDragon::AEndlessGravesDragon() : AEndlessGravesAICharacter()
{
	//CapsuleComp->BodyInstance.bLockXRotation = true;
	//CapsuleComp->BodyInstance.bLockYRotation = true;

	FireBallSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FireBallSpawnLocation->SetupAttachment(RootComponent);
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
	// DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 12, FColor::Red, false, 10.0f);

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

	OnPawnSeen(HeardPawn);
}

AEndlessGravesFireBall* AEndlessGravesDragon::GetAvailableFireBall()
{
	for (AEndlessGravesFireBall* FireBall : FireBallList)
	{
		if(FireBall->IsAvailable() == true)
			return FireBall;
	}

	return nullptr;
}

void AEndlessGravesDragon::AttackPlayer(FVector PlayerLocation)
{
	Super::AttackPlayer(PlayerLocation);
	ensure(FireBallClass != nullptr);
	FVector direction = PlayerLocation - GetActorLocation();
	const FVector SocketLocation = FireBallSpawnLocation->GetComponentLocation(); // GetMesh()->GetSocketLocation("Bone_002");
	direction.Normalize();

	// GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("EndlessGravesDragon AttackPlayer %s"), *(GetName())));

	FireBallActor = GetAvailableFireBall();
	if (FireBallActor == nullptr)
	{
		// GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("FireBallActor is NULL"));
		UWorld* const World = GetWorld();
		if (World)
		{
			// GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("FireBallActor is Spawn"));
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			
			FRotator spawnRotation = FireBallSpawnLocation->GetComponentRotation(); // FRotator::ZeroRotator;// FRotationMatrix::MakeFromX(direction).Rotator();
			//spawnRotation.Pitch = 30.0f;
			FireBallActor = World->SpawnActor<AEndlessGravesFireBall>(FireBallClass, SocketLocation, spawnRotation, ActorSpawnParams);

			FireBallList.Add(FireBallActor);
		}
	}
	if(FireBallActor)
		FireBallActor->Active(SocketLocation, direction);
}

void AEndlessGravesDragon::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("EndlessGravesDragon::OnBeginOverlap %s"), *(OtherActor->GetName())));

	IEndlessGravesWeaponInterface* Weapon = Cast<IEndlessGravesWeaponInterface>(OtherActor);
	if (Weapon && DamageImmunity == false)
	{ 
		CurHealth -= Weapon->GetDamage();
		GetWorldTimerManager().SetTimer(DamageImmunityTimeHandler, this, &AEndlessGravesDragon::UnlockDamageImmunity, 0.5f, false);
	}

	UpdateAIHUD();
}