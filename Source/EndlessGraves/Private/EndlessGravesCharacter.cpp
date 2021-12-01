// Copyright EndlessGraves, Inc. All Rights Reserved.


#include "EndlessGravesCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "EndlessGravesCharacterInfoWidget.h"
#include "EndlessGravesProjectile.h"

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

	if (HUDInfoWidget == nullptr)
		HUDInfoWidget = Cast<UEndlessGravesCharacterInfoWidget>(CreateWidget(GetWorld(), HUDInfoWidgetClass));
	if(HUDInfoWidget->IsInViewport() == false)
		HUDInfoWidget->AddToViewport();
}

// Called to bind functionality to input
void AEndlessGravesCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

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
		OnSlashSword();
	else
		OnFireArrow();
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
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// spawn the projectile at the muzzle
			World->SpawnActor<AEndlessGravesProjectile>(ArrowClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
	}
}

void AEndlessGravesCharacter::SwitchWeapon()
{
	if (CurWeapon == EWeaponType::WEAPON_ARROW)
		SwitchWeaponTo(EWeaponType::WEAPON_SWORD);
	else
		SwitchWeaponTo(EWeaponType::WEAPON_ARROW);
}

void AEndlessGravesCharacter::SwitchWeaponTo(EWeaponType weaponType)
{
	CurWeapon = weaponType;
	HUDInfoWidget->SwitchWeapon(CurWeapon);
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