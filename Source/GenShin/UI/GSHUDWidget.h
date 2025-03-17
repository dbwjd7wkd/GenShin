// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameData/GSCharacterStat.h"
#include "GSHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class GENSHIN_API UGSHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UGSHUDWidget(const FObjectInitializer& ObjectInitializer);

public:
	void UpdateStat(const FGSCharacterStat& BaseStat, const FGSCharacterStat& ModifierStat);
	void UpdateHpBar(float NewCurrentHp);

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	TObjectPtr<class UGSHpBarWidget> HpBar;

	UPROPERTY()
	TObjectPtr<class UGSCharacterStatWidget> CharacterStat;

};
