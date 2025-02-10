// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPSUserWidget.h"
#include "TestCompiledUEPlayerController.generated.h" 

class UInputMappingContext;

/**
 *
 */
UCLASS()
class TESTCOMPILEDUE_API ATestCompiledUEPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;

	// Begin Actor interface
protected:

	virtual void BeginPlay() override;

	// End Actor interface

protected:
	// UMG Widget 的蓝图类 (Reference to HUD)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UFPSUserWidget> MyWidgetClass;

private:
	UPROPERTY()
	UFPSUserWidget* MyWidgetInstance;

public:
	// 修改 Widget 文本
	void UpdateWidgetText(int32);
};
