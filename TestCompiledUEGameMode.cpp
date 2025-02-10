// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestCompiledUEGameMode.h"
#include "TestCompiledUECharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "TargetCube.h"
#include "EngineUtils.h" // TActorIterator �����ͷ�ļ�
#include "Math/UnrealMathUtility.h" // FMath::RandRange(min, max) ����������ͷ�ļ�������[min, max]�������
#include "GameFramework/PlayerState.h" // APlayerState �����ͷ�ļ�

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

	for (TActorIterator<AActor> It(GetWorld(), ATargetCube::StaticClass()); It; ++It) // �����������Actor
	{
		AActor* BlueprintInstance = *It;
		if (BlueprintInstance)
		{
			CubeSet.Add(Cast<ATargetCube>(BlueprintInstance)); // ��ӵ��б�Castȷ������ת����ȫ
			//UE_LOG(LogTemp, Log, TEXT("Found Blueprint instance: %s"), *BlueprintInstance->GetName());
		}
	}

	
	// ����Ƿ����㹻��Ԫ��
	if (N > CubeSet.Num()) {
		UE_LOG(LogTemp, Warning, TEXT("Not enough elements in the set!"));
		N = CubeSet.Num(); // ��ֹԽ��
	}
	
	UE_LOG(LogTemp, Log, TEXT("N = %d"), N);
	for (int32 i = 0; i < N; i++) { // ���N������Ϊ���ⷽ��
		int32 randNum = FMath::RandRange(0, CubeSet.Num() - 1);
		//CubeSet[randNum]->Health = 2;
		CubeSet[randNum]->targetPoint *= 2;
		if (CubeSet[randNum]->StaticMeshComponent && GoldMat) {
			CubeSet[randNum]->StaticMeshComponent->SetMaterial(0, GoldMat);
		}
		CubeSet.RemoveAt(randNum);
	}


	// ��ʼ������ʱ
	CountdownTime = TotalCountdownTime;

	// ������ʱ����ÿ�����һ�� UpdateCountdown
	GetWorld()->GetTimerManager().SetTimer(
		CountdownTimerHandle,
		this,
		&ATestCompiledUEGameMode::UpdateCountdown,
		1.0f,   // ÿ�����һ��
		true    // ѭ��
	);
}

void ATestCompiledUEGameMode::UpdateCountdown()
{
	// ���ٵ���ʱ
	CountdownTime--;

	// ��ӡ��ǰ����ʱ
	UE_LOG(LogTemp, Log, TEXT("Time Remaining: %f"), CountdownTime);

	// ����Ƿ����
	if (CountdownTime <= 0)
	{
		// ֹͣ��ʱ��
		GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);

		// ��������ʱ����¼�
		CountdownFinished();
	}
}

void ATestCompiledUEGameMode::CountdownFinished()
{
	// ����ʱ�����߼�
	UE_LOG(LogTemp, Log, TEXT("Countdown Finished!"));

	// ������ʵ������Զ����߼������紥��������ı�ؿ�״̬
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController && PlayerController->PlayerState) {
		// ��ȡ��ͼPlayerStateʵ��
		APlayerState* PlayerState = PlayerController->PlayerState;

		// ��ȡ�����ķ�����Ϣ
		FName VariableName(TEXT("Point")); // ��ͼPlayerState��������
		FProperty* Property = PlayerState->GetClass()->FindPropertyByName(VariableName);
		if (Property)
		{
			// ���������Ͳ���ȡֵ
			if (FIntProperty* IntProperty = CastField<FIntProperty>(Property))
			{
				// ��ӡ��ǰ����
				int32 PlayerPoint = IntProperty->GetPropertyValue_InContainer(PlayerState);
				UE_LOG(LogTemp, Log, TEXT("Final Score: %d"), PlayerPoint);
			}
		}
	}
}