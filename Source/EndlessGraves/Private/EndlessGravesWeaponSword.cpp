// Copyright EndlessGraves, Inc. All Rights Reserved.


#include "EndlessGravesWeaponSword.h"

// Sets default values
AEndlessGravesWeaponSword::AEndlessGravesWeaponSword()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEndlessGravesWeaponSword::BeginPlay()
{
	Super::BeginPlay();
	
}

float AEndlessGravesWeaponSword::GetDamage() const
{
	return 30.0f;
}

