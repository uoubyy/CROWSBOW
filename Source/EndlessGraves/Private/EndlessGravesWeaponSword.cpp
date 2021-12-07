// Copyright EndlessGraves, Inc. All Rights Reserved.


#include "EndlessGravesWeaponSword.h"
#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AEndlessGravesWeaponSword::AEndlessGravesWeaponSword()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = RootScene;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->AttachToComponent(RootScene, FAttachmentTransformRules::KeepRelativeTransform);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DStaticMeshComponent"));
	MeshComponent->AttachToComponent(RootScene, FAttachmentTransformRules::KeepRelativeTransform);

	Traileffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TrailEffect"));
	Traileffect->AttachToComponent(RootScene, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AEndlessGravesWeaponSword::BeginPlay()
{
	Super::BeginPlay();
	//Traileffect->SetVisibility(false);
	//CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AEndlessGravesWeaponSword::OnBeginOverlap);
	
}

float AEndlessGravesWeaponSword::GetDamage() const
{
	return Damage * DamageCoefficient;
}

void AEndlessGravesWeaponSword::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void AEndlessGravesWeaponSword::ActiveSword(bool enable)
{
	SetActorHiddenInGame(!enable);
	CapsuleComponent->SetCollisionEnabled(enable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
}

void AEndlessGravesWeaponSword::ChangeDamageCoe(float value)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("EndlessGravesWeaponSword::ChangeDamageCoe %f"), value));
	DamageCoefficient = value; 
}

void AEndlessGravesWeaponSword::ToggleTrailEffect(bool value)
{
	Traileffect->SetVisibility(value);
}