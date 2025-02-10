// Fill out your copyright notice in the Description page of Project Settings.


#include "IsPlayerInRange.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy_Controller.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyKeys.h"

UIsPlayerInRange::UIsPlayerInRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player In Range");
}

void UIsPlayerInRange::OnBecomeRelevant(UBehaviorTreeComponent& a_pTreeComp, uint8* a_pNodeMem)
{
	// Super Call
	Super::OnBecomeRelevant(a_pTreeComp, a_pNodeMem);

	// Get AI Controller
	AEnemy_Controller* pAIController = Cast<AEnemy_Controller>(a_pTreeComp.GetAIOwner());
	APawn* pEnemy = pAIController->GetPawn();

	// Get Player Controller
	APlayerController* pPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// Set blackboard key to whether or not player is in range
	pAIController->GetBlackboard()->SetValueAsBool(EnemyKeys::isPlayerInRange, pEnemy->GetDistanceTo(pPlayerController->GetPawn()) <= m_fRange);
}
