// Copyright Epic Games, Inc. All Rights Reserved.


#include "TestCompiledUEPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"

//#include "FPSUserWidget.h" // 包含自定义 UMG

void ATestCompiledUEPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}

	
	// add UMG UUserWidget
	// 检查是否设置了 Widget 的蓝图类
	if (MyWidgetClass)
	{
		// 创建 Widget 实例
		//UFPSUserWidget* MyWidget = CreateWidget<UFPSUserWidget>(this, MyWidgetClass);
		MyWidgetInstance = CreateWidget<UFPSUserWidget>(this, MyWidgetClass);

		// 检查 Widget 是否创建成功
		if (MyWidgetInstance)
		{
			// 将 Widget 添加到视口
			MyWidgetInstance->AddToViewport();
		}
	}

	// 创建并显示自定义 UMG
	//if (UClass* WidgetClass = LoadClass<UFPSUserWidget>(nullptr, TEXT("/Game/UI/MyUserWidget.MyUserWidget_C")))
	//{
	//	MyWidgetInstance = CreateWidget<UFPSUserWidget>(this, WidgetClass);
	//	if (MyWidgetInstance)
	//	{
	//		MyWidgetInstance->AddToViewport();
	//	}
	//}

	// 加载 UMG 蓝图
	//if (UClass* WidgetClass = LoadClass<UFPSUserWidget>(nullptr, TEXT("/Game/MyFPS/BP_FPSUserWidget.BP_FPSUserWidget_C")))
	//{
	//	UE_LOG(LogTemp, Log, TEXT("Got UMG!!!!!!!!!AAAAAAAAAAAAAAAAAAAAAaa"));
	//	MyWidgetInstance = CreateWidget<UFPSUserWidget>(this, WidgetClass);

	//	if (MyWidgetInstance)
	//	{
	//		MyWidgetInstance->AddToViewport();
	//	}
	//}
}

void ATestCompiledUEPlayerController::UpdateWidgetText(int32 score)
{
	if (MyWidgetInstance)
	{
		UE_LOG(LogTemp, Log, TEXT("Got MyWidgetInstance"));
		MyWidgetInstance->SetScore(score); UE_LOG(LogTemp, Log, TEXT("SetUI"));
	}
}