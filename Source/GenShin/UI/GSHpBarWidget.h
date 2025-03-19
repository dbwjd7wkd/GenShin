// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/GSUserWidget.h"
#include "GameData/GSCharacterStat.h"
#include "GSHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class GENSHIN_API UGSHpBarWidget : public UGSUserWidget
{
	GENERATED_BODY()

public:
	UGSHpBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void UpdateStat(const FGSCharacterStat& BaseStat, const FGSCharacterStat& ModifierStat);
	void UpdateHpBar(float NewCurrentHp);
	FString GetHpStatText();

public:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY()
	TObjectPtr<class UTextBlock> HpStat;

	UPROPERTY()
	float CurrentHp;

	UPROPERTY()
	float MaxHp;
};
