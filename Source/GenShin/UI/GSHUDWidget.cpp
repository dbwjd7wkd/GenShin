// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GSHUDWidget.h"
#include "Interface/GSCharacterHUDInterface.h"
#include "GSHpBarWidget.h"
#include "GSCharacterStatWidget.h"

UGSHUDWidget::UGSHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UGSHUDWidget::UpdateStat(const FGSCharacterStat& BaseStat, const FGSCharacterStat& ModifierStat)
{
	HpBar->UpdateStat(BaseStat, ModifierStat);

	CharacterStat->UpdateStat(BaseStat, ModifierStat);
}

void UGSHUDWidget::UpdateHpBar(float NewCurrentHp)
{
	HpBar->UpdateHpBar(NewCurrentHp);
}

void UGSHUDWidget::NativeConstruct()
{
	// �ʱ�ȭ

	Super::NativeConstruct();

	HpBar = Cast<UGSHpBarWidget>(GetWidgetFromName(TEXT("WidgetHpBar")));
	ensure(HpBar);

	CharacterStat = Cast<UGSCharacterStatWidget>(GetWidgetFromName(TEXT("WidgetCharacterStat")));
	ensure(CharacterStat);

	// HUD�� ���Ȱ� �ʱ�ȭ �� ��������Ʈ ����
	IGSCharacterHUDInterface* HUDPawn = Cast<IGSCharacterHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetupHUDWidget(this);
	}
}
