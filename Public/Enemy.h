// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyManager.h"
#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

UCLASS()
class TESTCOMPILEDUE_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// References to montages
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* m_pAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* m_pDeathMontage;

	// Reference to sounds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	TArray<USoundBase*> m_pAttackSounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	TArray<USoundBase*> m_pDeathSounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	USoundBase* m_pEnemyDamageSound;

	// Combat fields
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float m_fHealth = 30.0f;
	FVector m_vWorldSpawnLocation;
	bool m_bInCombat = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray< TSubclassOf<class AActor> > m_aWeaponDroped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DropWeaponRate = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AActor> m_aFirstAidKitDroped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DropKitRate = 0.15f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float m_fEnemyDamage = 10.0f;

	// Combat methods
	void Attack();

	void TakeDamage(float a_fDamage);

	//Death handling methods
	void Die();
	void Reset();

	// Montage End Event Handler
	UFUNCTION()
	void HandleOnMontageEnded(UAnimMontage* a_pMontage, bool a_bInterrupted);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& SweepResult);

	UFUNCTION()
	void DropWeapon();

	UFUNCTION()
	void DropFirstAidKit();

	UFUNCTION()
	void AddPoint();
	
private:
	// Reference that contains enemy manager
	AEnemyManager* m_pEnemyManager;
};
