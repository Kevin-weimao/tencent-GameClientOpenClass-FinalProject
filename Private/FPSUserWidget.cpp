// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSUserWidget.h"
#include "Components/TextBlock.h"
#include <Components/CanvasPanel.h>

void UFPSUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 检查组件是否成功绑定
    if (MyTextBlock)
    {
        //MyTextBlock->SetText(FText::FromString(TEXT("0")));
        SetScore(0);
    }
}

void UFPSUserWidget::SetScore(int32 score) {
    if (MyTextBlock)
    {
        MyTextBlock->SetText(FText::FromString(FString::FromInt(score)));
    }
}


