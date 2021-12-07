// Copyright EndlessGraves, Inc. All Rights Reserved.


#include "EndlessGravesGem.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstance.h"

void AEndlessGravesGem::Init(EGemType Type)
{
	GemType = Type;
	StaticMeshComponent->SetMaterial(0, GemMaterialMap[Type]);
}