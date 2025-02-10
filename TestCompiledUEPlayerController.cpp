// Copyright Epic Games, Inc. All Rights Reserved.


#include "TestCompiledUEPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"

//#include "FPSUserWidget.h" // �����Զ��� UMG

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
	// ����Ƿ������� Widget ����ͼ��
	if (MyWidgetClass)
	{
		// ���� Widget ʵ��
		//UFPSUserWidget* MyWidget = CreateWidget<UFPSUserWidget>(this, MyWidgetClass);
		MyWidgetInstance = CreateWidget<UFPSUserWidget>(this, MyWidgetClass);

		// ��� Widget �Ƿ񴴽��ɹ�
		if (MyWidgetInstance)
		{
			// �� Widget ��ӵ��ӿ�
			MyWidgetInstance->AddToViewport();
		}
	}

	// ��������ʾ�Զ��� UMG
	//if (UClass* WidgetClass = LoadClass<UFPSUserWidget>(nullptr, TEXT("/Game/UI/MyUserWidget.MyUserWidget_C")))
	//{
	//	MyWidgetInstance = CreateWidget<UFPSUserWidget>(this, WidgetClass);
	//	if (MyWidgetInstance)
	//	{
	//		MyWidgetInstance->AddToViewport();
	//	}
	//}

	// ���� UMG ��ͼ
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