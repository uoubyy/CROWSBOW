// Copyright EndlessGraves, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EndlessGravesAIController.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSGRAVES_API AEndlessGravesAIController : public AAIController
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, Category = AIController)
	FVector TargetLocation;
	
public:
	UFUNCTION(BlueprintCallable, Category = AIController)
	void ChasingPlayer(FVector Location);

	//UFUNCTION(BlueprintCallable, Category = AIController)
	//void 
};
