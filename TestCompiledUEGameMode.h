// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TestCompiledUEGameMode.generated.h"

UCLASS(minimalapi)
class ATestCompiledUEGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATestCompiledUEGameMode();

	UPROPERTY(EditAnywhere)
	int32 N = 5; // 需要生成特殊方块的个数
	UPROPERTY(EditAnywhere)
	float T = 30.0f; // 关卡时长
	UPROPERTY(EditAnywhere)
	UMaterial *GoldMat;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// 倒计时剩余时间
	float CountdownTime;

	// 倒计时总时间（可配置）
	UPROPERTY(EditAnywhere, Category = "Countdown")
	float TotalCountdownTime = 10.0f;

	// 定时器句柄
	FTimerHandle CountdownTimerHandle;

	// 倒计时更新函数
	void UpdateCountdown();

	// 倒计时完成函数
	void CountdownFinished();
};



