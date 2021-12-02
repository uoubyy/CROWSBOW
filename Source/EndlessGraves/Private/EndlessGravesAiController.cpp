// Copyright EndlessGraves, Inc. All Rights Reserved.


#include "EndlessGravesAIController.h"
#include "NavigationSystem.h"

void AEndlessGravesAIController::ChasingPlayer(FVector Location)
{
	TargetLocation = Location;
	MoveToLocation(TargetLocation, 50.0f);
}

void AEndlessGravesAIController::RandomMovement(FVector Origin)
{
	//UWorld* const World = GetWorld();
	//if (World)
	//{
	//	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());

	//	if (NavSys != nullptr && NavSys->K2_GetRandomPointInNavigableRadius(World, Origin, TargetLocation, 500.0f))
	//		MoveToLocation(TargetLocation, 50.0f);
	//}
}