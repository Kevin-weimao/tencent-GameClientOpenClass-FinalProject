// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "EnhancedInputComponent.h"
#include "TP_WeaponComponent.generated.h"

class ATestCompiledUECharacter;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTCOMPILEDUE_API UTP_WeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class ATestCompiledUEProjectile> ProjectileClass;

	// Weapon Sounds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* m_pFireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* m_pEmptySound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* m_pReloadSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* m_pAttachWeaponSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TArray<USoundBase*> m_pEnvironmentSounds;

	// Muzzle Offset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GamePlay")
	FVector m_vMuzzleOffset = FVector(100.f, 0, 10.f);

	// Muzzle Flash
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	UNiagaraSystem* m_pMuzzleFlash;
	
	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	/** Reload Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ReloadAction;

	/** Drop Weapon Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* DropAction;

	/* Grip Poit of skeletal mesh component */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName m_GripPointName = FName(TEXT("GripPoint"));

	/** Sets default values for this component's properties */
	UTP_WeaponComponent();

	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	bool AttachWeapon(ATestCompiledUECharacter* TargetCharacter);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void DettachWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Replenishment(ATestCompiledUECharacter* TargetCharacter);

	FEnhancedInputActionEventBinding* StartFireBindingHandle = nullptr;
	FEnhancedInputActionEventBinding* EndFireBindingHandle = nullptr;
	FEnhancedInputActionEventBinding* ReloadBindingHandle = nullptr;

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Fire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StartFire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void EndFire();

	void Reload();

	// Ammo sizes
	// Current ammo number in clip
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	int m_iClipSize = 8; // 当前弹夹中子弹数

	// Current clip capacity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	int m_iClipCapacity = 8; // 当前弹夹容量

	// Current ammo number holding
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	int m_iCurrentAmmo = 80; // 当前持有子弹数

	// Max ammo number holding
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	int m_iMaxAmmo = 80; // 该枪最大子弹数

	UFUNCTION()
	void HandleOnMontageEnd(UAnimMontage* a_pMontage, bool a_bInterrupted); // a: argument

	// Update displayed ammo number
	UFUNCTION()
	void UpdateAmmoNumDisplay();

	// Reload Montage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* m_pReloadMontage;

	FTimerHandle FireTimerHandle;  // 定时器句柄

	FTimerHandle StartFireTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	float IntervalTime = 0.5f;     // 时间间隔

	// Weapon type name and slate brush (UI)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName m_tWeaponName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* m_fWeaponIcon;

protected:
	/** Ends gameplay for this component. */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	/** The Character holding this weapon*/
	ATestCompiledUECharacter* Character;
	bool m_bCanStartFire = true;
};
