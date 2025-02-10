// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestCompiledUEGameMode.h"
#include "TestCompiledUECharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "TargetCube.h"
#include "EngineUtils.h" // TActorIterator 所需的头文件
#include "Math/UnrealMathUtility.h" // FMath::RandRange(min, max) 随机数所需的头文件，生成[min, max]的随机数
#include "GameFramework/PlayerState.h" // APlayerState 所需的头文件

ATestCompiledUEGameMode::ATestCompiledUEGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/BluePrints/Mao"));
	//DefaultPawnClass = PlayerPawnClassFinder.Class;

}

void ATestCompiledUEGameMode::BeginPlay() {
	TArray<ATargetCube*> CubeSet;

	for (TActorIterator<AActor> It(GetWorld(), ATargetCube::StaticClass()); It; ++It) // 遍历场景里的Actor
	{
		AActor* BlueprintInstance = *It;
		if (BlueprintInstance)
		{
			CubeSet.Add(Cast<ATargetCube>(BlueprintInstance)); // 添加到列表，Cast确保类型转换安全
			//UE_LOG(LogTemp, Log, TEXT("Found Blueprint instance: %s"), *BlueprintInstance->GetName());
		}
	}

	
	// 检查是否有足够的元素
	if (N > CubeSet.Num()) {
		UE_LOG(LogTemp, Warning, TEXT("Not enough elements in the set!"));
		N = CubeSet.Num(); // 防止越界
	}
	
	UE_LOG(LogTemp, Log, TEXT("N = %d"), N);
	for (int32 i = 0; i < N; i++) { // 随机N个方块为特殊方块
		int32 randNum = FMath::RandRange(0, CubeSet.Num() - 1);
		//CubeSet[randNum]->Health = 2;
		CubeSet[randNum]->targetPoint *= 2;
		if (CubeSet[randNum]->StaticMeshComponent && GoldMat) {
			CubeSet[randNum]->StaticMeshComponent->SetMaterial(0, GoldMat);
		}
		CubeSet.RemoveAt(randNum);
	}


	// 初始化倒计时
	CountdownTime = TotalCountdownTime;

	// 启动计时器，每秒调用一次 UpdateCountdown
	GetWorld()->GetTimerManager().SetTimer(
		CountdownTimerHandle,
		this,
		&ATestCompiledUEGameMode::UpdateCountdown,
		1.0f,   // 每秒调用一次
		true    // 循环
	);
}

void ATestCompiledUEGameMode::UpdateCountdown()
{
	// 减少倒计时
	CountdownTime--;

	// 打印当前倒计时
	UE_LOG(LogTemp, Log, TEXT("Time Remaining: %f"), CountdownTime);

	// 检查是否完成
	if (CountdownTime <= 0)
	{
		// 停止计时器
		GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);

		// 触发倒计时完成事件
		CountdownFinished();
	}
}

void ATestCompiledUEGameMode::CountdownFinished()
{
	// 倒计时结束逻辑
	UE_LOG(LogTemp, Log, TEXT("Countdown Finished!"));

	// 在这里实现你的自定义逻辑，例如触发动画或改变关卡状态
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController && PlayerController->PlayerState) {
		// 获取蓝图PlayerState实例
		APlayerState* PlayerState = PlayerController->PlayerState;

		// 获取变量的反射信息
		FName VariableName(TEXT("Point")); // 蓝图PlayerState变量名称
		FProperty* Property = PlayerState->GetClass()->FindPropertyByName(VariableName);
		if (Property)
		{
			// 检查变量类型并读取值
			if (FIntProperty* IntProperty = CastField<FIntProperty>(Property))
			{
				// 打印当前积分
				int32 PlayerPoint = IntProperty->GetPropertyValue_InContainer(PlayerState);
				UE_LOG(LogTemp, Log, TEXT("Final Score: %d"), PlayerPoint);
			}
		}
	}
}