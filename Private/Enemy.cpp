// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "TestCompiledUE/TestCompiledUECharacter.h"
#include "Components/CapsuleComponent.h"
#include "TestCompiledUE/TestCompiledUEProjectile.h"
#include "GameFramework/PlayerState.h"
#include "TestCompiledUE/TestCompiledUEPlayerController.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	// Bind Event of onhit by bullet
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AEnemy::OnHit);

	// Bind events
	GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &AEnemy::HandleOnMontageEnded);

	// Set world spawn location
	m_vWorldSpawnLocation = GetActorLocation();

	// Get reference to enemy manager
	m_pEnemyManager = Cast<AEnemyManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AEnemyManager::StaticClass()));
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::Attack()
{
	UAnimInstance* pAnimInstance = GetMesh()->GetAnimInstance();
	if (pAnimInstance != nullptr)
	{
		// Play attack montage
		if (m_pAttackMontage != nullptr)
		{
			pAnimInstance->Montage_Play(m_pAttackMontage);

			// Get reference to player
			ATestCompiledUECharacter* pPlayer = Cast<ATestCompiledUECharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			if (pPlayer != nullptr)
			{
				pPlayer->TakeDamage(m_fEnemyDamage);
			}

			// Play attack sounds
			if (m_pAttackSounds.Num() > 0)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_pAttackSounds[FMath::RandRange(0, m_pAttackSounds.Num() - 1)], GetActorLocation());
			}
		}
	}
}

void AEnemy::TakeDamage(float a_fDamage)
{
	// Check if death montage is playing
	UAnimInstance* pAnimInstance = GetMesh()->GetAnimInstance();
	if (pAnimInstance != nullptr)
	{
		if (pAnimInstance->Montage_IsPlaying(m_pDeathMontage))
		{
			return;
		}
	}

	// Play damage indicator
	if (m_pEnemyDamageSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_pEnemyDamageSound, GetActorLocation());
	}

	// Deal damage
	m_fHealth -= a_fDamage;
	//UE_LOG(LogTemp, Warning, TEXT("Enemy current health: %f"), m_fHealth);

	// Death handling
	if (m_fHealth <= 0)
	{
		Die();
	}
}

void AEnemy::Die()
{
	// Try and play death sounds
	if (m_pDeathSounds.Num() > 0) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_pDeathSounds[FMath::RandRange(0, m_pDeathSounds.Num() - 1)], GetActorLocation());
	}

	// Try and play death montage
	UAnimInstance* pAnimInstance = GetMesh()->GetAnimInstance();
	if (pAnimInstance != nullptr)
	{
		if (m_pDeathMontage != nullptr)
		{
			pAnimInstance->Montage_Play(m_pDeathMontage);
		}
	}

	// Add point
	AddPoint();

	// Drop items
	DropWeapon();
	DropFirstAidKit();

	// Set no collision
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));
}

void AEnemy::Reset()
{
	// Telepotr enemy back to spawn location
	TeleportTo(m_vWorldSpawnLocation, GetActorRotation());

	// Reset health and combat status
	m_fHealth = 30.0f;
	m_bInCombat = false;

	// Reset collision to "Pawn"
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
}

void AEnemy::HandleOnMontageEnded(UAnimMontage* a_pMontage, bool a_bInterrupted)
{
	// Chech for death montage
	if (a_pMontage->GetName().Contains("Death"))
	{
		Reset();
		
		// Update Enemy Manager
		m_pEnemyManager->m_iWaveKills++;

		if (m_pEnemyManager->m_iWaveKills >= m_pEnemyManager->m_iCurrentWaveSize)
		{
			// Begin the next wave and update params
			m_pEnemyManager->UpdateWaveParameters();
			m_pEnemyManager->StartNextWave();
		}
		else
		{
			// Try to spawn more enemies
			m_pEnemyManager->SpawnMoreEnemies();
		}
	}
}

void AEnemy::OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& SweepResult)
{
	ATestCompiledUEProjectile* Bullet = Cast<ATestCompiledUEProjectile>(OtherActor);
	if (Bullet != nullptr)
	{
		TakeDamage(Bullet->m_fDamage);
	}
}

void AEnemy::DropWeapon()
{
	float DropIndex = FMath::FRandRange(0.f, 1.f);
	//UE_LOG(LogTemp, Log, TEXT("Drop index: %f"), DropIndex);
	if (DropIndex < DropWeaponRate)
	{
		int WeaponTypeIndex = FMath::RandRange(0, m_aWeaponDroped.Num() - 1);
		//UE_LOG(LogTemp, Log, TEXT("Drop weapon %d"), WeaponTypeIndex);
		if (m_aWeaponDroped[WeaponTypeIndex] != nullptr)
		{
			UWorld* const World = GetWorld();
			FActorSpawnParameters WeaponSpawnParams;
			WeaponSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AActor* theWeaponPickupActor = World->SpawnActor<AActor>(m_aWeaponDroped[WeaponTypeIndex], GetActorLocation(), GetActorRotation(), WeaponSpawnParams);
			
			// Set simulate physics
			UTP_WeaponComponent* theWeapon = theWeaponPickupActor->GetComponentByClass<UTP_WeaponComponent>();
			if (theWeapon != nullptr)
			{
				//UE_LOG(LogTemp, Log, TEXT("Get Weapon"));
				UPrimitiveComponent* WeaponMesh = Cast<UPrimitiveComponent>(theWeapon->GetOwner()->GetComponentByClass(UPrimitiveComponent::StaticClass()));
				if (WeaponMesh)
				{
					//UE_LOG(LogTemp, Log, TEXT("Get Weapon Mesh"));
					WeaponMesh->SetSimulatePhysics(true);
					WeaponMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
				}
			}
		}
	}
}

void AEnemy::DropFirstAidKit()
{
	float DropIndex = FMath::RandRange(0.f, 1.f);
	if (DropIndex < DropKitRate && m_aFirstAidKitDroped != nullptr)
	{
		FActorSpawnParameters FirstAidKitSpawnParams;
		FirstAidKitSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<AActor>(m_aFirstAidKitDroped, GetActorLocation(), GetActorRotation(), FirstAidKitSpawnParams);
	}
}

void AEnemy::AddPoint()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController && PlayerController->PlayerState) {
		// 获取蓝图PlayerState实例
		APlayerState* LocalPlayerState = PlayerController->PlayerState;

		// 获取变量的反射信息
		FName VariableName(TEXT("Point")); // 蓝图PlayerState变量名称
		FProperty* Property = LocalPlayerState->GetClass()->FindPropertyByName(VariableName);
		if (Property)
		{
			// 检查变量类型并读取值
			if (FIntProperty* IntProperty = CastField<FIntProperty>(Property))
			{
				// 获得积分，并打印当前积分
				int32 PlayerPoint = IntProperty->GetPropertyValue_InContainer(LocalPlayerState);
				PlayerPoint += 5;
				IntProperty->SetPropertyValue_InContainer(LocalPlayerState, PlayerPoint);
				UE_LOG(LogTemp, Log, TEXT("Player Score: %d"), PlayerPoint);

				// 更新UI
				ATestCompiledUEPlayerController* MyController = Cast<ATestCompiledUEPlayerController>(GetWorld()->GetFirstPlayerController());
				if (MyController)
				{
					MyController->UpdateWidgetText(PlayerPoint);
				}
			}
		}
	}
}

