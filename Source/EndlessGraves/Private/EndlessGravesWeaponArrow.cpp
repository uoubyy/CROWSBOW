// Copyright EndlessGraves, Inc. All Rights Reserved.


#include "EndlessGravesWeaponArrow.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"

AEndlessGravesWeaponArrow::AEndlessGravesWeaponArrow() : AEndlessGravesProjectile()
{
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->AttachToComponent(RootScene, FAttachmentTransformRules::KeepRelativeTransform);

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	CapsuleComp->AttachToComponent(RootScene, FAttachmentTransformRules::KeepRelativeTransform);

	// TODO object pool
	InitialLifeSpan = 5.0f;
}


void AEndlessGravesWeaponArrow::BeginPlay()
{
	Super::BeginPlay();
	CapsuleComp->OnComponentHit.AddDynamic(this, &AEndlessGravesWeaponArrow::OnHit);
	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AEndlessGravesWeaponArrow::OnBeginOverlap);
}

void AEndlessGravesWeaponArrow::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Destroy();
}

void AEndlessGravesWeaponArrow::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("EndlessGravesWeaponArrow::OnBeginOverlap %s"), *(OtherActor->GetName())));

	Destroy();
}
