// Copyright EndlessGraves, Inc. All Rights Reserved.


#include "EndlessGravesAIController.h"
#include "NavigationSystem.h"

void AEndlessGravesAIController::ChasingPlayer(FVector Location)
{
	TargetLocation = Location;
	MoveToLocation(TargetLocation, 120.0f, false, true, true);
}

void AEndlessGravesAIController::RandomMovement(FVector Origin)
{

}
