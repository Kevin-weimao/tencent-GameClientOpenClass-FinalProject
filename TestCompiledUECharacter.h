// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Kismet/GameplayStatics.h"
#include "Styling/SlateBrush.h"
#include "Brushes/SlateImageBrush.h"
#include "Engine/Texture2D.h"
#include "TP_WeaponComponent.h"
#include "TestCompiledUECharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ATestCompiledUECharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Reload Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ReloadAction;
	
public:
	/* State whether the character has gun */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool hasGun;

	UTP_WeaponComponent* CurrentWeapon;

	// Public fields
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float m_fHealth = 100.0f;

	// Ammo sizes displayed on HUD
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int currentAmmo = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int totalAmmo = 0;

	ATestCompiledUECharacter();

protected:
	virtual void BeginPlay();

public:
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	// Reload Montage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* m_pReloadMontage;

	// Shooting Montage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* m_pShootMontage;

	// Method for taking damage
	void TakeDamage(float a_fDamage);

	// Method for reloading
	void Reload();

	// Defalut weapon type name and slate brush (UI)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FName m_tDefaultWeaponName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UTexture2D* m_fDefaultWeaponIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FName m_tCurrentWeaponName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UTexture2D* m_fCurrentWeaponIcon;

	// Current weapon type name and slate brush (UI)
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	//FName m_tCurrentName;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	//FSlateBrush* m_fCurrentWeaponSlateBrush;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Brush")
	//FSlateImageBrush* ImageBrush;

	// Method for setting UI weapon type
	UFUNCTION()
	void ResetUIWeaponType();
	UFUNCTION()
	void SetUIWeaponType(FName a_tWeaponName, UTexture2D* a_fWeaponSlateBrush);

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

