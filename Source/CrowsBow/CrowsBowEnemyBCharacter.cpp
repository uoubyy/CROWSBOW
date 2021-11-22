//  Copyright Yanyi Bao. All Rights Reserved.


#include "CrowsBowEnemyBCharacter.h"

// Sets default values
ACrowsBowEnemyBCharacter::ACrowsBowEnemyBCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CurEnemyState = EEnemyState::ES_Idle;
}

// Called when the game starts or when spawned
void ACrowsBowEnemyBCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

EEnemyState ACrowsBowEnemyBCharacter::GetEnemyState()
{
	return CurEnemyState;
}

void ACrowsBowEnemyBCharacter::OnPawnSeen(APawn* SeenPawn)
{

}

void ACrowsBowEnemyBCharacter::OnNoiseHeard(APawn* HeardPawn, const FVector& Location, float Volume)
{

}

void ACrowsBowEnemyBCharacter::TurnToSenseActor()
{

}

void ACrowsBowEnemyBCharacter::ChaseToSenseActor()
{

}

void ACrowsBowEnemyBCharacter::AttackPlayer(FVector tergetLoc)
{

}

