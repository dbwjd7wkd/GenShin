// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GSHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Interface/GSCharacterWidgetInterface.h"
#include "Character/GSCharacterBase.h"

UGSHpBarWidget::UGSHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1;
}

void UGSHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 앞에서 UI 관련 모든 기능들이 거의 초기화 됨.
	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpBar")));
	ensure(HpProgressBar);

	IGSCharacterWidgetInterface* GSCharacterWidgetInterface = Cast<IGSCharacterWidgetInterface>(OwningActor);
	if (GSCharacterWidgetInterface)
	{
		GSCharacterWidgetInterface->SetupCharacterWidget(this);
	}

}

void UGSHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	ensure(MaxHp > 0.0f);
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(NewCurrentHp / MaxHp);
	}

}
