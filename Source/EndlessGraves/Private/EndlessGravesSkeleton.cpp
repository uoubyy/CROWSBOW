// Copyright EndlessGraves, Inc. All Rights Reserved.


#include "EndlessGravesSkeleton.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "EndlessGravesWeaponInterface.h"
#include "EndlessGravesAIController.h"

#include "Kismet/KismetMathLibrary.h"

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
}

void AEndlessGravesSkeleton::OnPawnSeen(APawn* SeenPawn)
{
	Super::OnPawnSeen(SeenPawn);

	TargetDirection = SensedLocation - GetActorLocation();
	float Distance = TargetDirection.Size();

	// GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("AEndlessGravesSkeleton OnPawnSeen %f"), Distance));

	if (Distance <= MaxAttackDistance)
	{
		CurEnemyState = EEnemyState::ES_Attack1;
	}
	else
	{
		CurEnemyState = EEnemyState::ES_Running;
		AEndlessGravesAIController* AIController = Cast<AEndlessGravesAIController>(GetController());
		if (AIController)
			AIController->ChasingPlayer(SensedLocation);
	}

	TargetDirection.Normalize();
}

void AEndlessGravesSkeleton::OnNoiseHeard(APawn* HeardPawn, const FVector& Location, float Volume)
{
	Super::OnNoiseHeard(HeardPawn, Location, Volume);

	// TODO debug
	OnPawnSeen(HeardPawn);
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
		//FVector LaunchVeolocity = GetActorForwardVector() * 1000.0f;
		//LaunchCharacter(LaunchVeolocity, true, false);

		GetWorldTimerManager().SetTimer(DamageImmunityTimeHandler, this, &AEndlessGravesSkeleton::UnlockDamageImmunity, 0.5f, false);
	}

	UpdateAIHUD();
}

//
//void AEndlessGravesSkeleton::OnExitOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//
//}