// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_ChasePlayer.generated.h"

/**
 * 
 */
UCLASS()
class TESTCOMPILEDUE_API UBTT_ChasePlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTT_ChasePlayer(FObjectInitializer const& a_pObjectInit);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& a_pTreeComp, uint8* m_pNodeMem);
	
};
