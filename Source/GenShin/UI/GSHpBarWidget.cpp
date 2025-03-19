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

	// �տ��� UI ���� ��� ��ɵ��� ���� �ʱ�ȭ ��.
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
	// ������ ����� ������ ��������Ʈ�� ���� �̺�Ʈ�� �޾Ƽ� ������Ʈ�� ����� ��.

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
	// 100/100 ���·� �ؽ�Ʈ�� �ٷ� ��������� �Լ�

	return FString::Printf(TEXT("%.0f/%0.f"), CurrentHp, MaxHp);
}
