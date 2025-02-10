// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetCube.generated.h"

UCLASS()
class TESTCOMPILEDUE_API ATargetCube : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATargetCube();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(Category = "Variable", EditAnywhere)
	int32 Health = 2;
	UPROPERTY(Category = "Variable", EditAnywhere)
	int32 targetPoint = 1;
	UPROPERTY(Category = "Variable", EditAnywhere)
	float ScaleTime = 0.5f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& SweepResult);
	
	UFUNCTION()
	void ChangeScale();
};
