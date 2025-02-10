// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestCompiledUECharacter.h"
#include "TestCompiledUEProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ATestCompiledUECharacter

ATestCompiledUECharacter::ATestCompiledUECharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

}

void ATestCompiledUECharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	hasGun = false;
}

//////////////////////////////////////////////////////////////////////////// Input

void ATestCompiledUECharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Bind reload event
		// EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &ATestCompiledUECharacter::Reload);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATestCompiledUECharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATestCompiledUECharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ATestCompiledUECharacter::TakeDamage(float a_fDamage)
{
	// Deal damage
	m_fHealth -= a_fDamage;

	// Handle death
	if (m_fHealth <= 0)
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName(GetWorld()->GetName()), true);
	}
}

void ATestCompiledUECharacter::Reload()
{
	UAnimInstance* pAnimInstance = GetMesh1P()->GetAnimInstance();
	if (pAnimInstance != nullptr) {
		//Play reload montage
		pAnimInstance->Montage_Play(m_pReloadMontage, 1.f);
	}
}

void ATestCompiledUECharacter::ResetUIWeaponType()
{
	m_tCurrentWeaponName = m_tDefaultWeaponName;
	m_fCurrentWeaponIcon = m_fDefaultWeaponIcon;
}

void ATestCompiledUECharacter::SetUIWeaponType(FName a_tWeaponName, UTexture2D* a_fWeaponIcon)
{
	m_tCurrentWeaponName = a_tWeaponName;
	m_fCurrentWeaponIcon = a_fWeaponIcon;
}

void ATestCompiledUECharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ATestCompiledUECharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}