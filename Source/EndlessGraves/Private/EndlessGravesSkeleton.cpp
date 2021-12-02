// Copyright EndlessGraves, Inc. All Rights Reserved.


#include "EndlessGravesSkeleton.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "EndlessGravesWeaponInterface.h"

#include "Kismet/KismetMathLibrary.h"

AEndlessGravesSkeleton::AEndlessGravesSkeleton()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEndlessGravesSkeleton::BeginPlay()
{
	Super::BeginPlay();
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AEndlessGravesSkeleton::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AEndlessGravesSkeleton::OnNoiseHeard);

	//CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AEndlessGravesSkeleton::OnBeginOverlap);
	//CapsuleComp->OnComponentEndOverlap.AddDynamic(this, &AEndlessGravesSkeleton::OnExitOverlap);
}

void AEndlessGravesSkeleton::Tick(float DeltaSeconds)
{
	if (CurEnemyState == EEnemyState::ES_Running)
	{
		FVector newPos = UKismetMathLibrary::VInterpTo(GetActorLocation(), SensedLocation - TargetDirection * MaxAttackDistance, DeltaSeconds, DeltaSeconds * ChaseSpeed);
		newPos.Z = GetActorLocation().Z;
		SetActorLocation(newPos);
	}
}

void AEndlessGravesSkeleton::OnPawnSeen(APawn* SeenPawn)
{
	Super::OnPawnSeen(SeenPawn);

	TargetDirection = SensedLocation - GetActorLocation();
	TargetDirection.Normalize();

	CurEnemyState = EEnemyState::ES_Running;
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
	if (Weapon)
		CurHealth -= Weapon->GetDamage();

	UpdateAIHUD();
}
//
//void AEndlessGravesSkeleton::OnExitOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//
//}