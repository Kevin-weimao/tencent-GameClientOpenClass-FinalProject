// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestCompiledUEProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Enemy.h"

ATestCompiledUEProjectile::ATestCompiledUEProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ATestCompiledUEProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

	// Create bullet static mesh component
	m_pBulletStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletStaticMeshComponent"));

	// Attach bullet static mesh component to root components
	m_pBulletStaticMeshComponent->SetupAttachment(RootComponent);
}

void ATestCompiledUEProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		if (m_pBulletStaticMeshComponent != nullptr) {
			OtherComp->AddImpulseAtLocation(GetVelocity() * m_fMass, GetActorLocation());
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("BulletStaticMeshComponent is Empty!"));
		}

		
	}

	//if (Cast<AEnemy>(OtherActor) != nullptr)
	//{
	//	Destroy();
	//}

	Destroy();
}