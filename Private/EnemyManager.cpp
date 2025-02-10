// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"
#include "Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AEnemyManager::AEnemyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AEnemyManager::SpawnFirstWave()
{
	// Ensure that there are enemies in the last
	if (m_pEnemies.Num() > 0)
	{
		// Init spawn count for this wave
		int spawnCount = 0;

		// Iterate through all enemies
		for (int i = 0; i < m_pEnemies.Num(); i++)
		{
			// Break the loop if target number of enemies has been reched
			if (spawnCount == m_iFirstWaveSpawnCount)
			{
				break;
			}

			// Cast actor to enemy
			AEnemy* pEnemy = Cast<AEnemy>(m_pEnemies[i]);

			// Disable collisions temporarily in case enemies try to spawn on top of one another
			pEnemy->SetActorEnableCollision(false);
			pEnemy->TeleportTo(m_pSpawnLocations[FMath::RandRange(0, m_pSpawnLocations.Num() - 1)], pEnemy->GetActorRotation());
			pEnemy->SetActorEnableCollision(true);

			// Set combat status
			pEnemy->m_bInCombat = true;

			// Update spawn count
			spawnCount++;
		}
	}
}

void AEnemyManager::UpdateWaveParameters()
{
	// Update wave count and kills
	m_iCurrentWave += 1;
	m_iWaveKills = 0;

	// Update wave size and arena max
	m_iCurrentWaveSize += 4;
	m_iMaxEnemiesInArena += 2;
	if (m_iMaxEnemiesInArena >= m_pEnemies.Num())
	{
		m_iMaxEnemiesInArena = m_pEnemies.Num() - 2;
	}

	// Update max and min walk speeds
	m_fGlobalMaxWalkSpeed += 50.0f;
	if (m_fGlobalMaxWalkSpeed > 600.0f)
	{
		m_fGlobalMaxWalkSpeed = 600.0f;
	}

	m_fGlobalMinWalkSpeed += 25.0f;
	if (m_fGlobalMinWalkSpeed > 300.0f)
	{
		m_fGlobalMinWalkSpeed = 300.0f;
	}
}

void AEnemyManager::StartNextWave()
{
	// Init spawn count tracker
	int spawnCount = 0;

	// Iterate through enemies
	for (int i = 0; i < m_pEnemies.Num(); i++)
	{
		// Break loop if max enemies in arena has been reached or if full wave has been spawned
		if (spawnCount == m_iMaxEnemiesInArena || spawnCount == m_iCurrentWaveSize)
		{
			break;
		}

		// Cast to enemy
		AEnemy* pEnemy = Cast<AEnemy>(m_pEnemies[i]);

		// Check if enemy is in combat
		if (!pEnemy->m_bInCombat)
		{
			// Disable collisions temporarily in case enemies try to spawn on top of one another
			pEnemy->SetActorEnableCollision(false);
			pEnemy->TeleportTo(m_pSpawnLocations[FMath::RandRange(0, m_pSpawnLocations.Num() - 1)], pEnemy->GetActorRotation());
			pEnemy->SetActorEnableCollision(true);

			// Set combat status
			pEnemy->m_bInCombat = true;

			// Update spawn count
			spawnCount++;
		}
	}

	// Update wave speeds
	ModifyWaveSpeeds();
}

void AEnemyManager::SpawnMoreEnemies()
{
	// Check if enemy kills are needed for wave completion
	int neededKills = m_iCurrentWaveSize - m_iWaveKills;
	if (neededKills > 0)
	{
		if (GetAllEnemiesInCombat().Num() < neededKills)
		{
			// Check that arena isnt full
			if (GetAllEnemiesInCombat().Num() < m_iMaxEnemiesInArena)
			{
				// Iterate through enemies
				for (int i = 0; i < m_pEnemies.Num(); i++)
				{
					// Cast to enemy
					AEnemy* pEnemy = Cast<AEnemy>(m_pEnemies[i]);

					// Check if enemy is in combat
					if (!pEnemy->m_bInCombat)
					{
						// Disable collisions temporarily in case enemies try to spawn on top of one another
						pEnemy->SetActorEnableCollision(false);
						pEnemy->TeleportTo(m_pSpawnLocations[FMath::RandRange(0, m_pSpawnLocations.Num() - 1)], pEnemy->GetActorRotation());
						pEnemy->SetActorEnableCollision(true);

						// Set combat status
						pEnemy->m_bInCombat = true;
						break;
					}
				}
			}
		}
	}
}

TArray<AActor*> AEnemyManager::GetAllEnemiesInCombat()
{
	// Iterate through all enemies and add to another list all enemies in combat
	TArray<AActor*> pEnemiesInCombat = TArray<AActor*>();
	for (AActor* actor : m_pEnemies)
	{
		// Cast to enemy
		AEnemy* pEnemy = Cast<AEnemy>(actor);

		// Add to list if in-combat
		if (pEnemy->m_bInCombat)
		{
			pEnemiesInCombat.Add(actor);
		}
	}
	return pEnemiesInCombat;
}

void AEnemyManager::ModifyWaveSpeeds()
{
	// Iterate through all enemies
	for (int i = 0; i < m_pEnemies.Num(); i++)
	{
		// Cast to enemy
		AEnemy* pEnemy = Cast<AEnemy>(m_pEnemies[i]);

		// Update speed
		pEnemy->GetCharacterMovement()->MaxWalkSpeed = FMath::RandRange(m_fGlobalMinWalkSpeed, m_fGlobalMaxWalkSpeed);
	}
}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();
	
	// Get all instances of enemy actor and store in enemy list
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), m_pEnemies);

	// Delay spawning of enemies slightly
	FTimerHandle pTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(pTimerHandle, [&]()
	{
		// Spawn default amount of enemies
		SpawnFirstWave();

		// Modify wave speeds
		ModifyWaveSpeeds();

	}, 3, false);

	// Init wave size tracker
	m_iCurrentWaveSize = m_iFirstWaveSpawnCount;
}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

