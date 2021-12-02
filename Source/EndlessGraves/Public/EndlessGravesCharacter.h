// Copyright EndlessGraves, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "EndlessGravesCustomEnum.h"
#include "EndlessGravesCharacter.generated.h"

class USceneComponent;

UCLASS()
class ENDLESSGRAVES_API AEndlessGravesCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USceneComponent* ArrowSpawnLocation;

public:
	// Sets default values for this character's properties
	AEndlessGravesCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector ArrowOffset;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	UFUNCTION()
	void StartAttack();

	UFUNCTION()
	/** Fires a projectile. */
	void OnFireArrow();

	UFUNCTION(BlueprintImplementableEvent)
	void OnSlashSword();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	class UPawnNoiseEmitterComponent* NoiseEmitterComponent;

	UPROPERTY(EditDefaultsOnly, Category = Health)
	float MaxHealth = 600.0f;

	UPROPERTY(VisibleAnywhere, Category = Health)
	float CurHealth;

	UPROPERTY(EditDefaultsOnly, Category = Ability)
	int MaxArrowNum = 3;

	UPROPERTY(VisibleAnywhere, Category = Ability)
	int CurArrowNum;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	EWeaponType CurWeapon;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	float WeaponResumeInterval = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	TSubclassOf<class AEndlessGravesProjectile> ArrowClass;

	UPROPERTY(EditDefaultsOnly, Category = HUD)
	TSubclassOf<class UEndlessGravesCharacterInfoWidget> HUDInfoWidgetClass;

	UPROPERTY()
	class UEndlessGravesCharacterInfoWidget* HUDInfoWidget;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	TSubclassOf<class AEndlessGravesWeaponSword> SwordClass;

	UPROPERTY()
	class AEndlessGravesWeaponSword* SwordActor;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	class UAnimMontage* SlashSwordAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	class UAnimMontage* ShootArrowAnimMontage;

	UFUNCTION()
	void WeaponResume();

	FTimerHandle WeaponResumeHandler;

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintCallable)
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable)
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void SwitchWeapon();

	UFUNCTION(BlueprintCallable)
	void SwitchWeaponTo(EWeaponType weaponType);

private:
	UFUNCTION()
	void UpdateHUD();
};
