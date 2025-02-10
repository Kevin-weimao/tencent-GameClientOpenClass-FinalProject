// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <TestCompiledUE/TP_WeaponComponent.h>
//#include "NiagaraFunctionLibrary.h"
#include "Weapon.generated.h"

UCLASS()
class TESTCOMPILEDUE_API AWeapon : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	UStaticMeshComponent* m_pMesh;
	
public:	
	// Weapon Sounds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* m_pFireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* m_pEmptySound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TArray<USoundBase*> m_pEnvironmentSounds;

	// Muzzle Offset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GamePlay")
	FVector m_vMuzzleOffset = FVector(100.f, 0, 10.f);

	// Muzzle Flash
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	UNiagaraSystem* m_pMuzzleFlash;

	// Sets default values for this actor's properties
	AWeapon();

	// Weapon components
	UTP_WeaponComponent* m_pWeaponComponent;

	// Ammos sizes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	int m_iClipSize = 8;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	int m_iMaxAmmo = 80;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
