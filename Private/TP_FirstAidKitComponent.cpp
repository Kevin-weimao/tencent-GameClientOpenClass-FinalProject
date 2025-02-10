// Fill out your copyright notice in the Description page of Project Settings.


#include "TP_FirstAidKitComponent.h"
#include "Kismet/GameplayStatics.h"

void UTP_FirstAidKitComponent::RecoverCharacter(ATestCompiledUECharacter* a_aCharacter)
{
	if (a_aCharacter != nullptr)
	{
		a_aCharacter->m_fHealth = 100.0f;
		if (m_pPickUpSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_pPickUpSound, a_aCharacter->GetActorLocation());
		}
		GetOwner()->Destroy();
	}
}
