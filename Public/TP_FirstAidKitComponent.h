// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "TestCompiledUE/TestCompiledUECharacter.h"
#include "TP_FirstAidKitComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TESTCOMPILEDUE_API UTP_FirstAidKitComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void RecoverCharacter(ATestCompiledUECharacter* a_aCharacter);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* m_pPickUpSound;
};
