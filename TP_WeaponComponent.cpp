// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "TestCompiledUECharacter.h"
#include "TestCompiledUEProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "TP_PickUpComponent.h"
#include "Enemy.h"
#include "Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	m_vMuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
}


void UTP_WeaponComponent::Fire()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	// Check that our clip is not empty
	if (m_iClipSize <= 0)
	{
		return;
	}

	// Try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(m_vMuzzleOffset);
	
			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			//ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			// Set up Query params
			FCollisionQueryParams queryParams;
			queryParams.AddIgnoredActor(PlayerController->GetPawn());

			// Set up hit result
			FHitResult outHit;

			// Line Trace
			//bool bHit = World->LineTraceSingleByChannel(outHit, SpawnLocation, SpawnLocation + (SpawnRotation.Vector() * 3000), ECollisionChannel::ECC_Pawn, queryParams);
			bool bHit = World->LineTraceSingleByChannel(outHit, PlayerController->PlayerCameraManager->GetCameraLocation(), SpawnLocation + (SpawnRotation.Vector() * 3000), ECollisionChannel::ECC_Pawn, queryParams);
			//DrawDebugLine(World, SpawnLocation, SpawnLocation + (SpawnRotation.Vector() * 3000), FColor::Red, false, 5.f, 5, 5.f);

			// Spawn the projectile at the muzzle
			ATestCompiledUEProjectile* pProjectileBullet =
				World->SpawnActor<ATestCompiledUEProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

			//// Try to cast hit to an enemy
			//AEnemy* pEnemy = Cast<AEnemy>(outHit.GetActor());
			//if (pEnemy)
			//{
			//	pEnemy->TakeDamage(10.0f);
			//}
			//else // Play environmental sounds
			//{
			//	// Check there are sounds in list
			//	if (m_pEnvironmentSounds.Num() > 0)
			//	{
			//		UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_pEnvironmentSounds[FMath::RandRange(0, m_pEnvironmentSounds.Num() - 1)], Character->GetActorLocation());
			//	}
			//}

			// Reset speed if hit any component
			if (bHit)
			{
				if (pProjectileBullet != nullptr) {
					pProjectileBullet->ProjectileMovement->Velocity = (outHit.ImpactPoint - SpawnLocation).GetSafeNormal() * pProjectileBullet->ProjectileMovement->Velocity.Size();
					//UE_LOG(LogTemp, Log, TEXT("Impact Point: %f, %f, %f"), outHit.ImpactPoint.X, outHit.ImpactPoint.Y, outHit.ImpactPoint.Z);
					UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *outHit.GetActor()->GetName());
				}
			}


			// Update ammo count
			Character->currentAmmo -= 1;
			m_iClipSize -= 1;
		}

		// Try and spawn muzzle flash particle system
		if (m_pMuzzleFlash != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(m_vMuzzleOffset);

			UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, m_pMuzzleFlash, SpawnLocation, SpawnRotation);
		}
	}
	
	// Try and play the sound if specified
	if (m_pFireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, m_pFireSound, Character->GetActorLocation());
	}
	
	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			if (m_pReloadMontage != nullptr) {
				AnimInstance->Montage_Stop(0.2f, m_pReloadMontage); // 先停止正在播放的 Montage
			}
			//if (AnimInstance->IsAnyMontagePlaying())
			//{
			//	UE_LOG(LogTemp, Warning, TEXT("Another Montage is already playing!"));
			//}

			AnimInstance->Montage_Play(FireAnimation, 1.f);
			//UE_LOG(LogTemp, Log, TEXT("Play Fire Animation"));
		}
	}
}

void UTP_WeaponComponent::StartFire() {
	// 开始触发间隔调用
	//Fire();
	if (m_bCanStartFire)
	{
		GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &UTP_WeaponComponent::Fire, IntervalTime, true, 0);

		if (m_iClipSize <= 0 && m_pEmptySound != nullptr && Character != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, m_pEmptySound, Character->GetActorLocation());
		}

		m_bCanStartFire = false;
		GetWorld()->GetTimerManager().SetTimer(StartFireTimerHandle, [&]()
		{ 
			m_bCanStartFire = true;
			GetWorld()->GetTimerManager().ClearTimer(StartFireTimerHandle);

		}, IntervalTime, true);
	}
}

void UTP_WeaponComponent::EndFire() {
	// 停止触发
	GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
}

void UTP_WeaponComponent::Reload()
{
	// Check if reloading is necessary
	if (m_iClipSize >= m_iClipCapacity || m_iCurrentAmmo <= 0 || Character->GetMesh1P()->GetAnimInstance()->Montage_IsPlaying(m_pReloadMontage))
	{
		return;
	}

	UAnimInstance* pAnimInstance = Character->GetMesh1P()->GetAnimInstance();
	if (pAnimInstance != nullptr) {
		//Play reload montage
		pAnimInstance->Montage_Play(m_pReloadMontage, 1.f);
	}

	// Try and play the sound if specified
	if (m_pReloadSound != nullptr)
	{
		float AudioDuration = m_pReloadSound->Duration;
		float MontageDuration = m_pReloadMontage->GetPlayLength();
		float RequiredPitch = AudioDuration / MontageDuration;
		UGameplayStatics::PlaySoundAtLocation(this, m_pReloadSound, Character->GetActorLocation(), 1.0f, RequiredPitch);
	}
}

void UTP_WeaponComponent::HandleOnMontageEnd(UAnimMontage* a_pMontage, bool a_bInterrupted)
{
	// Get reload montage logic
	if (a_pMontage->GetName().Contains("reload") && !a_bInterrupted)
	{
		// Check that weapon is equipped
		if (Character && !Character->hasGun)
		{
			return;
		}

		if (!Character) {
			return;
		}

		// Check how much we need to reload and do reload
		if (m_iCurrentAmmo >= m_iClipCapacity - m_iClipSize) // 子弹够用
		{
			//Character->totalAmmo -= m_iClipCapacity;
			//Character->currentAmmo += m_iClipCapacity;

			m_iCurrentAmmo -= (m_iClipCapacity - m_iClipSize);
			m_iClipSize = m_iClipCapacity;
		}
		else // 剩余子弹数 少于 补充弹夹所需子弹数
		{
			//Character->totalAmmo -= Character->totalAmmo;
			//Character->currentAmmo += Character->totalAmmo;

			m_iClipSize += m_iCurrentAmmo;
			m_iCurrentAmmo = 0;
		}

		UpdateAmmoNumDisplay();
	}
}

void UTP_WeaponComponent::UpdateAmmoNumDisplay()
{
	if (Character)
	{
		Character->currentAmmo = m_iClipSize;
		Character->totalAmmo = m_iCurrentAmmo;
	}
}

bool UTP_WeaponComponent::AttachWeapon(ATestCompiledUECharacter* TargetCharacter)
{
	Character = TargetCharacter;

	// Check that the character is valid, and has no weapon component yet
	if (Character == nullptr || Character->GetInstanceComponents().FindItemByClass<UTP_WeaponComponent>() || Character->hasGun)
	{
		UE_LOG(LogTemp, Log, TEXT("You already have a gun!"));
		return false;
	}

	// Bind Reload Montage Events
	Character->GetMesh1P()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &UTP_WeaponComponent::HandleOnMontageEnd);

	// 不允许物理模拟，让武器掉落
	UPrimitiveComponent* WeaponMesh = Cast<UPrimitiveComponent>(GetOwner()->GetComponentByClass(UPrimitiveComponent::StaticClass()));
	if (WeaponMesh)
	{
		WeaponMesh->SetSimulatePhysics(false);
		//WeaponMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		UE_LOG(LogTemp, Log, TEXT("Get Weapon Mesh!"));
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, m_GripPointName);

	// add the weapon as an instance component to the character
	Character->AddInstanceComponent(this);

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			// 半自动
			//EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Fire);

			// 全自动
			StartFireBindingHandle = &EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &UTP_WeaponComponent::StartFire);
			EndFireBindingHandle = &EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &UTP_WeaponComponent::EndFire);
			// Bind reload event
			ReloadBindingHandle = &EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &UTP_WeaponComponent::Reload);
			// Bind drop weapon event
			EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::DettachWeapon);
		}
	}

	// 不允许物理模拟，让武器掉落
	WeaponMesh = Cast<UPrimitiveComponent>(GetOwner()->GetComponentByClass(UPrimitiveComponent::StaticClass()));
	if (WeaponMesh)
	{
		WeaponMesh->SetSimulatePhysics(false);
		//WeaponMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		UE_LOG(LogTemp, Log, TEXT("Get Weapon Mesh!"));
	}

	Character->hasGun = true;
	Character->CurrentWeapon = this;

	// Set ammo amount
	UpdateAmmoNumDisplay();

	// Set UI weapon type
	Character->SetUIWeaponType(m_tWeaponName, m_fWeaponIcon);

	// Try and play attach weapon sound
	if (m_pAttachWeaponSound != nullptr && Character != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, m_pAttachWeaponSound, Character->GetActorLocation());
	}

	return true;
}

void UTP_WeaponComponent::DettachWeapon()
{
	if (Character == nullptr)
	{
		return;
	}

	// 清除当前角色的子弹显示 (后续做：重新设置要丢出的武器拾取物中的子弹数量)
	Character->currentAmmo = 0;
	Character->totalAmmo = 0;

	// Reset UI weapon type
	Character->ResetUIWeaponType();

	// 解除输入绑定
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			EnhancedInputComponent->RemoveBinding(*StartFireBindingHandle);
			EnhancedInputComponent->RemoveBinding(*EndFireBindingHandle);
			EnhancedInputComponent->RemoveBinding(*ReloadBindingHandle);
		}
	}

	// 重新设置为可拾取
	if (UTP_PickUpComponent* WeaponPickup = this->GetOwner()->GetComponentByClass<UTP_PickUpComponent>())
	{
		WeaponPickup->SetPickable();
	}

	// 从角色实例组件中移除武器
	Character->RemoveInstanceComponent(this);

	// 解除父子关系
	DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

	// 允许物理模拟，让武器掉落
	UPrimitiveComponent* WeaponMesh = Cast<UPrimitiveComponent>(GetOwner()->GetComponentByClass(UPrimitiveComponent::StaticClass()));
	if (WeaponMesh)
	{
		WeaponMesh->SetSimulatePhysics(true);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		WeaponMesh->AddImpulse(Character->GetActorForwardVector() * 500.0f, NAME_None, true);
	}

	Character->hasGun = false;
	Character->CurrentWeapon = nullptr;

	// Remove Reload Montage Events
	Character->GetMesh1P()->GetAnimInstance()->OnMontageEnded.RemoveAll(this);

	// 清空角色引用
	Character = nullptr;
}

void UTP_WeaponComponent::Replenishment(ATestCompiledUECharacter* TargetCharacter)
{
	if (TargetCharacter != nullptr)
	{
		if (TargetCharacter->CurrentWeapon != nullptr)
		{
			if (TargetCharacter->CurrentWeapon->m_tWeaponName == m_tWeaponName)
			{
				if (Character == nullptr)
				{
					TargetCharacter->CurrentWeapon->m_iCurrentAmmo += (m_iCurrentAmmo + m_iClipSize);
					m_iCurrentAmmo = m_iClipSize = 0;
					TargetCharacter->totalAmmo = TargetCharacter->CurrentWeapon->m_iCurrentAmmo;
					// Try and play attach weapon sound
					if (m_pAttachWeaponSound != nullptr)
					{
						UGameplayStatics::PlaySoundAtLocation(this, m_pAttachWeaponSound, TargetCharacter->GetActorLocation());
					}
					this->GetOwner()->Destroy();
				}
			}
		}
	}
}

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}