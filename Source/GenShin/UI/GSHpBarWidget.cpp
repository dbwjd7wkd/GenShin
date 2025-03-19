// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GSHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Interface/GSCharacterWidgetInterface.h"
#include "Components/TextBlock.h"

UGSHpBarWidget::UGSHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1.0f;
}

void UGSHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 앞에서 UI 관련 모든 기능들이 거의 초기화 됨.
	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpBar")));
	ensure(HpProgressBar);

	HpStat = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtHpStat")));
	ensure(HpStat);

	IGSCharacterWidgetInterface* GSCharacterWidgetInterface = Cast<IGSCharacterWidgetInterface>(OwningActor);
	if (GSCharacterWidgetInterface)
	{
		GSCharacterWidgetInterface->SetupCharacterWidget(this);
	}

}

void UGSHpBarWidget::UpdateStat(const FGSCharacterStat& BaseStat, const FGSCharacterStat& ModifierStat)
{
	MaxHp = (BaseStat + ModifierStat).MaxHp;

	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(CurrentHp / MaxHp);
	}

	if (HpStat)
	{
		HpStat->SetText(FText::FromString(GetHpStatText()));
	}
}

void UGSHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	// 스탯이 변경될 떄마다 델리게이트로 부터 이벤트를 받아서 업데이트를 해줘야 함.

	CurrentHp = NewCurrentHp;

	ensure(MaxHp > 0.0f);
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(CurrentHp / MaxHp);
	}

	if (HpStat)
	{
		HpStat->SetText(FText::FromString(GetHpStatText()));
	}

}

FString UGSHpBarWidget::GetHpStatText()
{
	// 100/100 형태로 텍스트가 바로 만들어지는 함수

	return FString::Printf(TEXT("%.0f/%0.f"), CurrentHp, MaxHp);
}
