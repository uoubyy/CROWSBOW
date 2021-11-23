//  Copyright Yanyi Bao. All Rights Reserved.


#include "CrowsBowEnemyBCharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include <Components/WidgetComponent.h>
#include "CrowsBowHealthBarWidget.h"

// Sets default values
ACrowsBowEnemyBCharacter::ACrowsBowEnemyBCharacter()
{
	SetActorTickEnabled(true);
	PrimaryActorTick.bCanEverTick = true;

	CurEnemyState = EEnemyState::ES_Idle;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	HealthWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthWidgetComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CurHealth = MaxHealth;
}

// Called when the game starts or when spawned
void ACrowsBowEnemyBCharacter::BeginPlay()
{
	Super::BeginPlay();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &ACrowsBowEnemyBCharacter::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &ACrowsBowEnemyBCharacter::OnNoiseHeard);

	//EnemyMovementComp = Cast<UCharacterMovementComponent>(GetComponentByClass(UCharacterMovementComponent::StaticClass()));

	SetActorTickEnabled(false);

	HealthBar = Cast<UCrowsBowHealthBarWidget>(HealthWidgetComp->GetUserWidgetObject());
	HealthBar->UpdateHealthPercentage(1.0f);
}

void ACrowsBowEnemyBCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetWorldTimerManager().IsTimerActive(ChasingTimeHandler))
	{
		FVector newPos = UKismetMathLibrary::VInterpTo(GetActorLocation(), TargetLocation - TargetDirection * MaxAttackDistance, DeltaSeconds, DeltaSeconds * ChaseSpeed);
		SetActorLocation(newPos);
	}

	SetActorRotation(FMath::Lerp(GetActorRotation(), TargetDirection.Rotation(), 0.05f));

}

EEnemyState ACrowsBowEnemyBCharacter::GetEnemyState()
{
	return CurEnemyState;
}

void ACrowsBowEnemyBCharacter::OnPawnSeen(APawn* SeenPawn)
{
	//DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 12, FColor::Red, false, 10.0f);

	if (SeenPawn == nullptr)
		return;

	TargetLocation = SeenPawn->GetActorLocation();
	TargetDirection = TargetLocation - GetActorLocation();
	TargetDirection.Normalize();

	if ((TargetLocation - GetActorLocation()).Size() <= MaxAttackDistance && GetWorldTimerManager().IsTimerActive(ChasingTimeHandler))
	{
		CurEnemyState = EEnemyState::ES_Attack;
		GetWorldTimerManager().ClearTimer(ChasingTimeHandler);
		TargetDirection = FVector::ZeroVector;
		return;
	}

	CurEnemyState = EEnemyState::ES_Running;

	if(GetWorldTimerManager().IsTimerActive(ChasingTimeHandler) == false)
		GetWorldTimerManager().SetTimer(ChasingTimeHandler, this, &ACrowsBowEnemyBCharacter::ChaseToSenseActor, 0.05f, true);
}

void ACrowsBowEnemyBCharacter::OnNoiseHeard(APawn* HeardPawn, const FVector& Location, float Volume)
{
	
	//DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Yellow, false, 10.0f);

	TargetLocation = Location;
	TargetDirection = Location - GetActorLocation();
	TargetDirection.Normalize();

	if (GetWorldTimerManager().IsTimerActive(ChasingTimeHandler) == false)
		GetWorldTimerManager().SetTimer(TurningTimerHandle, this, &ACrowsBowEnemyBCharacter::TurnToSenseActor, 0.05f, true);

	SetActorTickEnabled(true);
}

void ACrowsBowEnemyBCharacter::TurnToSenseActor()
{
	SetActorRotation(FMath::Lerp(GetActorRotation(), TargetDirection.Rotation(), 0.05f));

	if (PawnSensingComp->CanSenseAnything() == false)
	{
		GetWorldTimerManager().ClearTimer(TurningTimerHandle);

		CurEnemyState = EEnemyState::ES_Idle;
		SetActorTickEnabled(false);
	}
}

void ACrowsBowEnemyBCharacter::ChaseToSenseActor()
{
	CurEnemyState = EEnemyState::ES_Running;

	if (PawnSensingComp->CanSenseAnything() == false)
	{
		CurEnemyState = EEnemyState::ES_Idle;
		GetWorldTimerManager().ClearTimer(ChasingTimeHandler);
		SetActorTickEnabled(false);
		return;
	}

	SetActorTickEnabled(true);
}

void ACrowsBowEnemyBCharacter::AttackPlayer(FVector tergetLoc)
{

}

void ACrowsBowEnemyBCharacter::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(0, 1.5f, FColor::Red, "Enemy OnHit");

	if(OtherComp->ComponentHasTag("Sword") && OtherComp->IsVisible())
	{
		CurHealth -= 3; // TODO config
	}

	HealthBar->UpdateHealthPercentage(CurHealth / MaxHealth);

	if (CurHealth <= 0)
		Destroy();
}
