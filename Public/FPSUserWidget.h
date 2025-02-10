// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h" // �����������
#include "FPSUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class TESTCOMPILEDUE_API UFPSUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    // ��ʼ�� UI �ĺ���
    virtual void NativeConstruct() override;

    void SetScore(int32);

protected:
    // ����ͼ�е����
    UPROPERTY(meta = (BindWidget))
    UTextBlock* MyTextBlock;


};
