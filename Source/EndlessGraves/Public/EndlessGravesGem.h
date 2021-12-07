// Copyright EndlessGraves, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EndlessGravesPickable.h"

#include "EndlessGravesCustomEnum.h"

#include "EndlessGravesGem.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSGRAVES_API AEndlessGravesGem : public AEndlessGravesPickable
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GemType)
	TMap<EGemType, UMaterialInstance*> GemMaterialMap;

protected:
	EGemType GemType;

public:
	EGemType GetGemType() const { return GemType;}

	UFUNCTION(BlueprintCallable)
	void Init(EGemType Type);
	
};
