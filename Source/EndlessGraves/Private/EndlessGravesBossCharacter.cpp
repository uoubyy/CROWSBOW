// Copyright EndlessGraves, Inc. All Rights Reserved.


#include "EndlessGravesBossCharacter.h"
#include "EndlessGravesWeaponInterface.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"

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
			GenerateNewState();
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
	//if (CurEnemyState == EEnemyState::ES_Attack1 || CurEnemyState == EEnemyState::ES_Attack2)
	//{
	//	return Damage;
	//}

	//return 0.0f;
	return Damage;
}

void AEndlessGravesBossCharacter::OnPawnSeen(APawn* SeenPawn)
{
	// Super::OnPawnSeen(SeenPawn);
}

void AEndlessGravesBossCharacter::OnNoiseHeard(APawn* HeardPawn, const FVector& Location, float Volume)
{
	Super::OnNoiseHeard(HeardPawn, Location, Volume);

	// TODO debug
	OnPawnSeen(HeardPawn);

	if((Location - GetActorLocation()).Size() <= MaxAttackDistance && CanAttackPlayer == false)
	{ 
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("AEndlessGravesBossCharacter change into attack"));
		ChangeStateInto(EEnemyState::ES_Attack1);
		CanAttackPlayer = true;
	}
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

void AEndlessGravesBossCharacter::TurnToSenseActor()
{
	FVector direction = SensedLocation - GetActorLocation();
	direction.Normalize();
	FRotator rotation = direction.Rotation();
	rotation.Roll = 0.0f;
	rotation.Pitch = 0.0f; // only rotation around the Z axis

	FName boneName("Head_M");
	//GetMesh()->SetBoneRotationByName(boneName, r, EBoneSpaces::ComponentSpace);
}