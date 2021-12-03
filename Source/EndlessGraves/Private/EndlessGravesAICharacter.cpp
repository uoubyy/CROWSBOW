// Copyright EndlessGraves, Inc. All Rights Reserved.


#include "EndlessGravesAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "UI/EndlessGravesHealthBarWidget.h"
#include "EndlessGravesCharacter.h"
#include "EndlessGravesAIController.h"

#include "Kismet/KismetMathLibrary.h"

// Sets default values
AEndlessGravesAICharacter::AEndlessGravesAICharacter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;
	//CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	//CapsuleComp->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	//RootComponent = CapsuleComp;

	//SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
	//SkeletalMeshComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	//SkeletalMeshComp->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	//SkeletalMeshComp->SetRelativeLocation(FVector::ZeroVector);

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->SetPeripheralVisionAngle(30.0f);
	PawnSensingComp->HearingThreshold = 1000.0f;
	PawnSensingComp->LOSHearingThreshold = 1200.0f;
	PawnSensingComp->SightRadius = 800.0f;

	HealthWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthWidgetComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FClassFinder<UUserWidget> HealthBarBP(TEXT("/Game/EndlessGraves/UI/BP_HealthBar"));
	HealthWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	HealthWidgetComp->SetWidgetClass(HealthBarBP.Class);

	CurEnemyState = EEnemyState::ES_Idle;
}

// Called when the game starts or when spawned
void AEndlessGravesAICharacter::BeginPlay()
{
	Super::BeginPlay();
	CurHealth = MaxHealth;
	HealthBar = Cast<UEndlessGravesHealthBarWidget>(HealthWidgetComp->GetUserWidgetObject());
	HealthWidgetComp->SetVisibility(false);
}

void AEndlessGravesAICharacter::OnPawnSeen(APawn* SeenPawn)
{
	if (SeenPawn == nullptr)
		return;

	AEndlessGravesCharacter* MainCharacter = Cast<AEndlessGravesCharacter>(SeenPawn);
	if (MainCharacter)
	{
		SensedLocation = SeenPawn->GetActorLocation();
	}
}

void AEndlessGravesAICharacter::OnNoiseHeard(APawn* HeardPawn, const FVector& Location, float Volume)
{
	// TODO filter careless pawn
	// for now, only player makes noise
	SensedLocation = Location;
	// if the timer exists and is active
	if(GetWorldTimerManager().IsTimerActive(TurningTimerHandle) == false)
		GetWorldTimerManager().SetTimer(TurningTimerHandle, this, &AEndlessGravesAICharacter::TurnToSenseActor, 0.05f, true);

	HealthWidgetComp->SetVisibility(true);
}

void AEndlessGravesAICharacter::OnPawnLost()
{
	CurEnemyState = EEnemyState::ES_Idle;
	HealthWidgetComp->SetVisibility(false);
}

void AEndlessGravesAICharacter::AttackPlayer(FVector PlayerLocation)
{

}

void AEndlessGravesAICharacter::TurnToSenseActor()
{
	FVector direction = SensedLocation - GetActorLocation();
	direction.Normalize();
	FRotator rotation = direction.Rotation();
	rotation.Roll = 0.0f;
	rotation.Pitch = 0.0f; // only rotation around the Z axis

	SetActorRotation(FMath::Lerp(GetActorRotation(), direction.Rotation(), 0.05f));

	if (PawnSensingComp->CanSenseAnything() == false)
	{
		GetWorldTimerManager().ClearTimer(TurningTimerHandle);
		OnPawnLost();
	}
}

void AEndlessGravesAICharacter::UpdateAIHUD()
{
	ensure(HealthBar != nullptr);
	HealthBar->SetHealthPercentage(CurHealth / MaxHealth);

	if (CurHealth <= 0)
		Destroy();
}

void AEndlessGravesAICharacter::MoveToLocation(FVector Location)
{
	AEndlessGravesAIController* PController = Cast<AEndlessGravesAIController>(Controller);
	if(PController)
		PController->ChasingPlayer(Location);
}