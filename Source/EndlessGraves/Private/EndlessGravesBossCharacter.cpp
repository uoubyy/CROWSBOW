// Copyright EndlessGraves, Inc. All Rights Reserved.


#include "EndlessGravesBossCharacter.h"

// Sets default values
AEndlessGravesBossCharacter::AEndlessGravesBossCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEndlessGravesBossCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEndlessGravesBossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEndlessGravesBossCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

