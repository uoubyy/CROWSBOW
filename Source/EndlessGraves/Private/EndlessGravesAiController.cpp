// Copyright EndlessGraves, Inc. All Rights Reserved.


#include "EndlessGravesAIController.h"
#include "NavigationSystem.h"

void AEndlessGravesAIController::ChasingPlayer(FVector Location)
{
	TargetLocation = Location;
	MoveToLocation(TargetLocation, 120.0f, false, true, true);
}

bool AEndlessGravesAIController::TryStartWander()
{
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	FNavLocation NavLocation;
	bool result = NavSys->GetRandomReachablePointInRadius(GetPawn()->GetActorLocation(), 1000.0f, NavLocation);
	if(result)
	{ 
		TargetLocation = NavLocation.Location;
		MoveToLocation(TargetLocation);
	}

	return result;
}
