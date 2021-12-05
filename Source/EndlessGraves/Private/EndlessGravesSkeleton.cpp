// Copyright EndlessGraves, Inc. All Rights Reserved.


#include "EndlessGravesSkeleton.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "EndlessGravesWeaponInterface.h"
#include "EndlessGravesAIController.h"

#include "Kismet/KismetMathLibrary.h"

#include "DrawDebugHelpers.h"

AEndlessGravesSkeleton::AEndlessGravesSkeleton() : AEndlessGravesAICharacter()
{
}

void AEndlessGravesSkeleton::BeginPlay()
{
	Super::BeginPlay();
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AEndlessGravesSkeleton::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AEndlessGravesSkeleton::OnNoiseHeard);

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEndlessGravesSkeleton::OnBeginOverlap);
	//CapsuleComp->OnComponentEndOverlap.AddDynamic(this, &AEndlessGravesSkeleton::OnExitOverlap);

	StateDuration = 3.0f;// TODO optimize
	ChangeStateInto(EEnemyState::ES_Wander);
}

void AEndlessGravesSkeleton::OnPawnSeen(APawn* SeenPawn)
{
	Super::OnPawnSeen(SeenPawn);

	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 12, FColor::Red, false, 10.0f);

	TargetDirection = SensedLocation - GetActorLocation();
	float Distance = TargetDirection.Size();

	// GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("AEndlessGravesSkeleton OnPawnSeen %f"), Distance));

	if (Distance <= MaxAttackDistance)
	{
		ChangeStateInto(EEnemyState::ES_Attack1);
	}
	else
	{
		ChangeStateInto(EEnemyState::ES_Running);
		AEndlessGravesAIController* AIController = Cast<AEndlessGravesAIController>(GetController());
		if (AIController)
			AIController->ChasingPlayer(SeenPawn->GetActorLocation());
	}

	TargetDirection.Normalize();
}

void AEndlessGravesSkeleton::OnNoiseHeard(APawn* HeardPawn, const FVector& Location, float Volume)
{
	Super::OnNoiseHeard(HeardPawn, Location, Volume);

	ChangeStateInto(EEnemyState::ES_Alert);

	if (GetWorldTimerManager().IsTimerActive(TurningTimerHandle) == false)
		GetWorldTimerManager().SetTimer(TurningTimerHandle, this, &AEndlessGravesSkeleton::TurnToSenseActor, 0.05f, true);

	DrawDebugSphere(GetWorld(), HeardPawn->GetActorLocation(), 32.0f, 12, FColor::Yellow, false, 10.0f);
}

void AEndlessGravesSkeleton::AttackPlayer(FVector PlayerLocation)
{

}

void AEndlessGravesSkeleton::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("EndlessGravesSkeleton::OnBeginOverlap %s"), *(OtherActor->GetName())));

	IEndlessGravesWeaponInterface* Weapon = Cast<IEndlessGravesWeaponInterface>(OtherActor);
	if (Weapon && DamageImmunity == false)
	{
		CurHealth -= Weapon->GetDamage() * 0.6;

		GetWorldTimerManager().SetTimer(DamageImmunityTimeHandler, this, &AEndlessGravesSkeleton::UnlockDamageImmunity, 0.5f, false);
	}

	UpdateAIHUD();
}

void AEndlessGravesSkeleton::ChangeStateInto(EEnemyState newState)
{
	if(CurEnemyState == newState)
		return;

	float leftTime = GetWorldTimerManager().GetTimerElapsed(ChangeStateHandle);

	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("Change from %s Into %s, duration %f"), *(GetEnumValueAsString<EEnemyState>("EEnemyState", CurEnemyState)),
		*(GetEnumValueAsString<EEnemyState>("EEnemyState", newState)), leftTime));

	Super::ChangeStateInto(newState);

	AEndlessGravesAIController* PController = Cast<AEndlessGravesAIController>(GetController());

	switch (newState)
	{
	case EEnemyState::ES_Idle:
		PController->StopMovement();
		GetWorldTimerManager().SetTimer(ChangeStateHandle, this, &AEndlessGravesSkeleton::GenerateNewState, StateDuration, false, StateDuration);
		break;
	case EEnemyState::ES_Wander:
		GetWorldTimerManager().SetTimer(ChangeStateHandle, this, &AEndlessGravesSkeleton::GenerateNewState, StateDuration, false, StateDuration);
		if(PController->TryStartWander() == false)
			CurEnemyState = EEnemyState::ES_Idle;
		break;
	case EEnemyState::ES_Running:
	case EEnemyState::ES_Attack1:
	case EEnemyState::ES_Attack2:
	case EEnemyState::ES_Alert:
	{
		GetWorldTimerManager().ClearTimer(ChangeStateHandle);
		PController->StopMovement();
		break;
	}
	case EEnemyState::ES_None:
		break;
	default:
		break;
	}
}

void AEndlessGravesSkeleton::GenerateNewState()
{
	StateDuration = FMath::RandRange(3.0f, 5.0f);

	ChangeStateInto(CurEnemyState == EEnemyState::ES_Idle ? EEnemyState::ES_Wander : EEnemyState::ES_Idle);
}