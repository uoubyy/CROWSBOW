// Copyright EndlessGraves, Inc. All Rights Reserved.


#include "EndlessGravesCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "GenericPlatform/GenericPlatformMath.h"

#include "Animation/AnimInstance.h"

#include "EndlessGravesCharacterInfoWidget.h"
#include "EndlessGravesProjectile.h"
#include "EndlessGravesWeaponInterface.h"
#include "EndlessGravesExtraHealth.h"
#include "EndlessGravesWeaponSword.h"
#include "EndlessGravesPlayerController.h"
#include "EndlessGravesBossCharacter.h"

// Sets default values
AEndlessGravesCharacter::AEndlessGravesCharacter()
{
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Arrow Spwan Location
	ArrowSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	ArrowSpawnLocation->SetupAttachment(RootComponent);
	ArrowSpawnLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	NoiseEmitterComponent = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("NoiseEmitter"));

	CurHealth = MaxHealth;
	CurArrowNum = MaxArrowNum;

	CurWeapon = EWeaponType::WEAPON_ARROW;
}

// Called when the game starts or when spawned
void AEndlessGravesCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AEndlessGravesCharacter::OnHit);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEndlessGravesCharacter::OnBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AEndlessGravesCharacter::OnExitOverlap);

	if (SwordClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			SwordActor = World->SpawnActor<AEndlessGravesWeaponSword>(SwordClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);
			SwordActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("SwordSocket"));
			SwordActor->SetActorRelativeLocation(FVector::ZeroVector);
		}
	}

	SwordActor->ActiveSword(CurWeapon == EWeaponType::WEAPON_SWORD);
	SwordActor->ChangeDamageCoe(0.0f);
}

void AEndlessGravesCharacter::ChangeDamageCoe(float value)
{
	SwordActor->ChangeDamageCoe(value);
}


void AEndlessGravesCharacter::OnStartGame()
{
	if (HUDInfoWidget == nullptr)
		HUDInfoWidget = Cast<UEndlessGravesCharacterInfoWidget>(CreateWidget(GetWorld(), HUDInfoWidgetClass));
	if(HUDInfoWidget->IsInViewport() == false)
		HUDInfoWidget->AddToViewport();

	AEndlessGravesPlayerController* PController = Cast<AEndlessGravesPlayerController>(Controller);
	if(PController)
		EnableInput(PController);
}

void AEndlessGravesCharacter::OnGameOver()
{
	if(HUDInfoWidget->IsInViewport())
		HUDInfoWidget->RemoveFromParent();

	AEndlessGravesPlayerController* PController = Cast<AEndlessGravesPlayerController>(Controller);
	if (PController)
		DisableInput(PController);
}

// Called to bind functionality to input
void AEndlessGravesCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AEndlessGravesCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AEndlessGravesCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AEndlessGravesCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AEndlessGravesCharacter::LookUpAtRate);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AEndlessGravesCharacter::StartAttack);
	// Bind switch weapon event
	PlayerInputComponent->BindAction("SwitchWeapon", IE_Pressed, this, &AEndlessGravesCharacter::SwitchWeapon);

	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AEndlessGravesCharacter::DashForward);

	PlayerInputComponent->BindAction<FOpenYawDelegate>("OpenYawController", IE_Pressed, this, &AEndlessGravesCharacter::OpenPlayerYawController, true);
	PlayerInputComponent->BindAction<FOpenYawDelegate>("OpenYawController", IE_Released, this, &AEndlessGravesCharacter::OpenPlayerYawController, false);
}

void AEndlessGravesCharacter::OpenPlayerYawController(bool value)
{
	bUseControllerRotationYaw = value;
	// GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, value ? TEXT("OpenPlayerYawController True") : TEXT("OpenPlayerYawController False"));
}

void AEndlessGravesCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AEndlessGravesCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AEndlessGravesCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AEndlessGravesCharacter::DashForward()
{
	FVector LaunchVeolocity = GetActorForwardVector() * 3000.0f;
	LaunchCharacter(LaunchVeolocity, true, false);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	GetWorldTimerManager().SetTimer(DashEffectTimeHandle, this, &AEndlessGravesCharacter::DashEffectExit, 0.5f, false);
}

void AEndlessGravesCharacter::DashEffectExit()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AEndlessGravesCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AEndlessGravesCharacter::StartAttack()
{
	if (CurWeapon == EWeaponType::WEAPON_SWORD)
	{
		// OnSlashSword();
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance->Montage_IsPlaying(SlashSwordAnimMontage) == false)
			AnimInstance->Montage_Play(SlashSwordAnimMontage);
	}
	else
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(ShootArrowAnimMontage);
		OnFireArrow();
	}
}

void AEndlessGravesCharacter::OnFireArrow()
{
	if (CurArrowNum <= 0 || HUDInfoWidget == nullptr)
		return;

	CurArrowNum--;
	HUDInfoWidget->UpdateArrowCD(CurArrowNum);

	if(GetWorldTimerManager().IsTimerActive(WeaponResumeHandler) == false || GetWorldTimerManager().IsTimerPaused(WeaponResumeHandler))
		GetWorldTimerManager().SetTimer(WeaponResumeHandler, this, &AEndlessGravesCharacter::WeaponResume, WeaponResumeInterval, true);

	if (ArrowClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			const FRotator SpawnRotation = GetActorRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = ((ArrowSpawnLocation != nullptr) ? ArrowSpawnLocation->GetComponentLocation() : GetActorLocation());// +SpawnRotation.RotateVector(ArrowOffset);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;// ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// spawn the projectile at the muzzle
			AEndlessGravesProjectile* arrow = World->SpawnActor<AEndlessGravesProjectile>(ArrowClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

			//if(arrow == nullptr)
			//	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("EndlessGravesCharacter::OnFireArrow failed"));
		}
	}
}

void AEndlessGravesCharacter::SwitchWeapon()
{
	if (CurWeapon == EWeaponType::WEAPON_ARROW)
		SwitchWeaponTo(EWeaponType::WEAPON_SWORD);
	else
		SwitchWeaponTo(EWeaponType::WEAPON_ARROW);

	OnSwitchWeaponBP();
}

void AEndlessGravesCharacter::SwitchWeaponTo(EWeaponType weaponType)
{
	CurWeapon = weaponType;
	HUDInfoWidget->SwitchWeapon(CurWeapon);

	SwordActor->ActiveSword(CurWeapon == EWeaponType::WEAPON_SWORD);
}

void AEndlessGravesCharacter::WeaponResume()
{
	if(CurWeapon == EWeaponType::WEAPON_ARROW && CurArrowNum < MaxArrowNum)
	{
		CurArrowNum++;

		if (CurArrowNum >= MaxArrowNum)
			GetWorldTimerManager().PauseTimer(WeaponResumeHandler);

		HUDInfoWidget->UpdateArrowCD(CurArrowNum);
	}
}

void AEndlessGravesCharacter::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// TODO
	// skeleton on hit bug
	// remove skeleton sword into single weapon
	// disable this for now 
	//IEndlessGravesWeaponInterface* Weapon = Cast<IEndlessGravesWeaponInterface>(OtherActor);
	//if (Weapon)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("AEndlessGravesCharacter OnHit %s"), *(OtherActor->GetName())));

	//	CurHealth -= Weapon->GetDamage();
	//}

	//UpdateHUD();
}

void AEndlessGravesCharacter::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	IEndlessGravesPowerUpInterface* PowerUp = Cast<IEndlessGravesPowerUpInterface>(OtherActor);
	if (PowerUp)
	{
		EPowerUpType PowerUpType = PowerUp->GetPowerUpType();
		switch (PowerUpType)
		{
		case EPowerUpType::POWERUP_ADDHEALTH:
			CurHealth += PowerUp->GetPowerUpEffectValue();
			break;
		case EPowerUpType::POWERUP_NONE:
			break;
		default:
			break;
		}
	}

	if (!DamageImmunity)
	{
		DamageImmunity = true;
		IEndlessGravesWeaponInterface* Weapon = Cast<IEndlessGravesWeaponInterface>(OtherActor);

		if (Weapon)
		{
			// GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("AEndlessGravesCharacter OnBeginOverlap Weapon %s"), *(OtherActor->GetName())));

			switch (Weapon->GetDamageType())
			{
			case EDamageType::EDT_Constant:
				ConstantDamageCount++;
				if (ConstantDamageCount == 1)
				{
					ConstantDamageDel.BindUFunction(this, FName("ConstantDamage"), Weapon->GetDamage());
					GetWorldTimerManager().SetTimer(ConstantDamageTimeHandle, ConstantDamageDel, 0.5f, true);
				}
				break;
			default:
				CurHealth -= Weapon->GetDamage();
				CamShakeEffect(1.0f);
				break;
			}

			GetWorldTimerManager().SetTimer(DamageImmunityTimeHandler, this, &AEndlessGravesCharacter::UnlockDamageImmunity, 0.2f, false);
		}
		else
		{
			// check hit result
			// GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("AEndlessGravesCharacter Bone Name %s"), *(SweepResult.BoneName.ToString())));
			// TODO configurable
			AEndlessGravesBossCharacter* Boss = Cast<AEndlessGravesBossCharacter>(OtherActor);
			if (Boss && BossDamageBoneList.Contains(SweepResult.BoneName.ToString()))
			{
				CurHealth -= Boss->GetDamage();
			}

			GetWorldTimerManager().SetTimer(DamageImmunityTimeHandler, this, &AEndlessGravesCharacter::UnlockDamageImmunity, 0.2f, false);
		}
	}
	UpdateHUD();
}

void AEndlessGravesCharacter::OnExitOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("AEndlessGravesCharacter OnExitOverlap %s"), *(OtherActor->GetName())));

	IEndlessGravesWeaponInterface* Weapon = Cast<IEndlessGravesWeaponInterface>(OtherActor);
	if (Weapon)
	{
		switch (Weapon->GetDamageType())
		{
		case EDamageType::EDT_Constant:
			ConstantDamageCount--; // No overlap stay event, maintain it by ourself
			if (ConstantDamageCount <= 0)
			{
				GetWorldTimerManager().ClearTimer(ConstantDamageTimeHandle);
			}
			break;
		default:
			break;
		}
	}
}


void AEndlessGravesCharacter::ConstantDamage(float Damage)
{
	CurHealth -= Damage;
	UpdateHUD();
}

void AEndlessGravesCharacter::UpdateHUD()
{
	if (CurHealth <= 0)
	{
		AEndlessGravesPlayerController* PController = Cast<AEndlessGravesPlayerController>(Controller);
		if (PController)
			PController->OnGameOver();
	}

	CurHealth = FMath::Clamp(CurHealth, 0.0f, MaxHealth);
	if (HUDInfoWidget)
	{
		int curLife = FGenericPlatformMath::CeilToInt(CurHealth / MaxHealth * 3);
		HUDInfoWidget->UpdateLifeStatus(curLife, CurHealth / MaxHealth);
	}
}

void AEndlessGravesCharacter::CamShakeEffect(float Scale)
{
	AEndlessGravesPlayerController* PController = Cast<AEndlessGravesPlayerController>(Controller);
	if(PController)
		PController->PlayerCameraManager->StartCameraShake(CamShake, Scale);
}