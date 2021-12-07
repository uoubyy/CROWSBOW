// Copyright EndlessGraves, Inc. All Rights Reserved.


#include "EndlessGravesBossCharacter.h"
#include "EndlessGravesWeaponInterface.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"

#include "EndlessGravesAIController.h"
#include "EndlessGravesPlayerController.h"
#include "Kismet/GameplayStatics.h"

#include "DrawDebugHelpers.h"

// Sets default values
AEndlessGravesBossCharacter::AEndlessGravesBossCharacter() : AEndlessGravesAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEndlessGravesBossCharacter::BeginPlay()
{
	Super::BeginPlay();
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AEndlessGravesBossCharacter::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AEndlessGravesBossCharacter::OnNoiseHeard);
	GetMesh()->OnComponentBeginOverlap.AddDynamic(this, &AEndlessGravesBossCharacter::OnBeginOverlap);

	HealthWidgetComp->SetVisibility(true);
}

void AEndlessGravesBossCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(true);
	if (CanAttackPlayer)
	{
		CurStateTime += DeltaSeconds;
		if (CurStateTime > StateDuration)
		{
			float posibility = FMath::RandRange(0.0f, 1.0f);
			ChangeStateInto(posibility > 0.5f ? EEnemyState::ES_Attack1 : EEnemyState::ES_Attack2);
		}
	}
}

void AEndlessGravesBossCharacter::GenerateNewState()
{
	StateDuration = FMath::RandRange(3.0f, 5.0f);

	if (CurEnemyState != EEnemyState::ES_Idle)
		ChangeStateInto(EEnemyState::ES_Idle);
	else
		ChangeStateInto(static_cast<EEnemyState>(FMath::RandRange(0, 4)));
}

float AEndlessGravesBossCharacter::GetDamage()
{
	if(CurEnemyState == EEnemyState::ES_Attack1 || CurEnemyState == EEnemyState::ES_Attack2)
		return Damage;
	else
		return 0.0f;
}

void AEndlessGravesBossCharacter::OnPawnSeen(APawn* SeenPawn)
{
	Super::OnPawnSeen(SeenPawn);

	float Distance = (SensedLocation - GetActorLocation()).Size();

	if (Distance <= MaxAttackDistance)
	{
		if (CanAttackPlayer == false)
		{
			StateDuration = FMath::RandRange(3.0f, 5.0f);
			float posibility = FMath::RandRange(0.0f, 1.0f);
			ChangeStateInto(posibility > 0.5f ? EEnemyState::ES_Attack1 : EEnemyState::ES_Attack2);

			DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 12, FColor::Red, false, 10.0f);

			CanAttackPlayer = true;
		}
	}
	else
	{
		CanAttackPlayer = false;
		ChangeStateInto(EEnemyState::ES_Running);
		AEndlessGravesAIController* AIController = Cast<AEndlessGravesAIController>(GetController());
		if (AIController)
			AIController->ChasingPlayer(SeenPawn->GetActorLocation());
	}
}

void AEndlessGravesBossCharacter::OnNoiseHeard(APawn* HeardPawn, const FVector& Location, float Volume)
{
	Super::OnNoiseHeard(HeardPawn, Location, Volume);

	ChangeStateInto(EEnemyState::ES_Alert);

	if (GetWorldTimerManager().IsTimerActive(TurningTimerHandle) == false)
		GetWorldTimerManager().SetTimer(TurningTimerHandle, this, &AEndlessGravesBossCharacter::TurnToSenseActor, 0.05f, true);

	DrawDebugSphere(GetWorld(), HeardPawn->GetActorLocation(), 32.0f, 12, FColor::Yellow, false, 10.0f);
}

void AEndlessGravesBossCharacter::OnPawnLost()
{
	Super::OnPawnLost();
	
	CanAttackPlayer = false;
}

void AEndlessGravesBossCharacter::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (DamageImmunity == false)
	{
		IEndlessGravesWeaponInterface* Weapon = Cast<IEndlessGravesWeaponInterface>(OtherActor);
		if (Weapon)
		{
			CurHealth -= Weapon->GetDamage() * 0.3f;
			DamageImmunity = true;

			GetWorldTimerManager().SetTimer(DamageImmunityTimeHandler, this, &AEndlessGravesBossCharacter::UnlockDamageImmunity, 0.5f, false);
		}

		UpdateAIHUD();
	}
}

void AEndlessGravesBossCharacter::UpdateAIHUD()
{
	Super::UpdateAIHUD();

	if(CurHealth <= 0.0f)
	{ 
		AEndlessGravesPlayerController* PController = Cast<AEndlessGravesPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (PController)
		{
			PController->OnGameOver(true);
		}
	}
}