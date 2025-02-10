// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"


// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_pMesh = CreateDefaultSubobject<UStaticMeshComponent>("Weapon Mesh");
	m_pMesh->SetupAttachment(RootComponent);

	m_pWeaponComponent = CreateDefaultSubobject<UTP_WeaponComponent>("Weapon Component");
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	// Assign weapon component variables
	m_pWeaponComponent->m_pFireSound = m_pFireSound;
	m_pWeaponComponent->m_pEmptySound = m_pEmptySound;
	m_pWeaponComponent->m_pEnvironmentSounds = m_pEnvironmentSounds;
	m_pWeaponComponent->m_vMuzzleOffset = m_vMuzzleOffset;
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

