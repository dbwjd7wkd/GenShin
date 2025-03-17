// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GSCharacterStatWidget.h"
#include "Components/TextBlock.h"

void UGSCharacterStatWidget::NativeConstruct()
{
	// BaseStat과 ModifierStat 각각의 룩업테이블이라는 맵구조에 (스탯의 속성값, 속성값에 대한 위젯의 텍스블록 포인터)들을 넣음.

	Super::NativeConstruct();

	for (TFieldIterator<FNumericProperty> PropIt(FGSCharacterStat::StaticStruct()); PropIt; ++PropIt)
	{
		const FName PropKey(PropIt->GetName());
		const FName TextBaseControlName = *FString::Printf(TEXT("Txt%sBase"), *PropIt->GetName());
		const FName TextModifierControlName = *FString::Printf(TEXT("Txt%sModifier"), *PropIt->GetName());

		UTextBlock* BaseTextBlock = Cast<UTextBlock>(GetWidgetFromName(TextBaseControlName));
		if (BaseTextBlock)
		{
			BaseLookup.Add(PropKey, BaseTextBlock);
		}

		UTextBlock* ModifierTextBlock = Cast<UTextBlock>(GetWidgetFromName(TextModifierControlName));
		if (ModifierTextBlock)
		{
			ModifierLookup.Add(PropKey, ModifierTextBlock);
		}
	}
}

void UGSCharacterStatWidget::UpdateStat(const FGSCharacterStat& BaseStat, const FGSCharacterStat& ModifierStat)
{
	// 업데이트된 스탯값을 위젯에 넣음.

	for (TFieldIterator<FNumericProperty> PropIt(FGSCharacterStat::StaticStruct()); PropIt; ++PropIt)
	{
		const FName PropKey(PropIt->GetName());

		float BaseData = 0.0f;
		PropIt->GetValue_InContainer((const void*)&BaseStat, &BaseData);
		float ModifierData = 0.0f;
		PropIt->GetValue_InContainer((const void*)&ModifierStat, &ModifierData);

		UTextBlock** BaseTextBlockPtr = BaseLookup.Find(PropKey);
		if (BaseTextBlockPtr)
		{
			(*BaseTextBlockPtr)->SetText(FText::FromString(FString::SanitizeFloat(BaseData)));
		}

		UTextBlock** ModifierTextBlockPtr = ModifierLookup.Find(PropKey);
		if (ModifierTextBlockPtr)
		{
			(*ModifierTextBlockPtr)->SetText(FText::FromString(FString::SanitizeFloat(ModifierData)));
		}
	}
}
