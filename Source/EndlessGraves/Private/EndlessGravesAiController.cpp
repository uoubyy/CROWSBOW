// Copyright EndlessGraves, Inc. All Rights Reserved.


#include "EndlessGravesAIController.h"

void AEndlessGravesAIController::ChasingPlayer(FVector Location)
{
	TargetLocation = Location;
	MoveToLocation(TargetLocation);
}