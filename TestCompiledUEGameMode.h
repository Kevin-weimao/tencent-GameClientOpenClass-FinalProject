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
	int32 N = 5; // ��Ҫ�������ⷽ��ĸ���
	UPROPERTY(EditAnywhere)
	float T = 30.0f; // �ؿ�ʱ��
	UPROPERTY(EditAnywhere)
	UMaterial *GoldMat;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// ����ʱʣ��ʱ��
	float CountdownTime;

	// ����ʱ��ʱ�䣨�����ã�
	UPROPERTY(EditAnywhere, Category = "Countdown")
	float TotalCountdownTime = 10.0f;

	// ��ʱ�����
	FTimerHandle CountdownTimerHandle;

	// ����ʱ���º���
	void UpdateCountdown();

	// ����ʱ��ɺ���
	void CountdownFinished();
};



