// Fill out your copyright notice in the Description page of Project Settings.


#include "CrowsBowAICharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"

#include "DrawDebugHelpers.h"

// Sets default values
ACrowsBowAICharacter::ACrowsBowAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BatEyesMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BatEyesMesh"));
	BatEyesMesh->AttachTo(GetMesh(), FName(TEXT("Face")), EAttachLocation::KeepRelativeOffset, false);

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	GetCharacterMovement()->GravityScale = 0;
}

// Called when the game starts or when spawned
void ACrowsBowAICharacter::BeginPlay()
{
	Super::BeginPlay();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &ACrowsBowAICharacter::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &ACrowsBowAICharacter::OnNoiseHeard);
	
}

void ACrowsBowAICharacter::OnPawnSeen(APawn* SeenPawn)
{
	if (SeenPawn == nullptr)
		return;

	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 12, FColor::Red, false, 10.0f);
}

void ACrowsBowAICharacter::OnNoiseHeard(APawn* HeardPawn, const FVector& Location, float Volume)
{
	DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Yellow, false, 10.0f);
}

// Called every frame
void ACrowsBowAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

