// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ChasePlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "Enemy_Controller.h"
#include "EnemyKeys.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"

UBTT_ChasePlayer::UBTT_ChasePlayer(FObjectInitializer const& a_pObjectInit)
{
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UBTT_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& a_pTreeComp, uint8* m_pNodeMem)
{
	// Get AI Controller
	auto const pAIController = Cast<AEnemy_Controller>(a_pTreeComp.GetAIOwner());

	// Get Nav System
	UNavigationSystemV1* pNavSystem = UNavigationSystemV1::GetCurrent(GetWorld());

	if (pNavSystem)
	{
		// Get location of the player
		FVector pLocation = pAIController->GetBlackboard()->GetValueAsVector(EnemyKeys::targetLocation);

		// Tell the AI to move towards player
		pAIController->MoveToLocation(pLocation);
	}

	// Finish execution
	FinishLatentTask(a_pTreeComp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}
