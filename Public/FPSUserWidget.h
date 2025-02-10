// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h" // 用于文字组件
#include "FPSUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class TESTCOMPILEDUE_API UFPSUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    // 初始化 UI 的函数
    virtual void NativeConstruct() override;

    void SetScore(int32);

protected:
    // 绑定蓝图中的组件
    UPROPERTY(meta = (BindWidget))
    UTextBlock* MyTextBlock;


};
