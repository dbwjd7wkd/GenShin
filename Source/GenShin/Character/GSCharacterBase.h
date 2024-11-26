// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GSCharacterBase.generated.h"

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};

UCLASS()
class GENSHIN_API AGSCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AGSCharacterBase();

	// Character Control sSection
protected:
	virtual void SetCharacterControlData(const class UGSCharacterControlData* CharacterControlData);

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, TObjectPtr<class UGSCharacterControlData>> CharacterControlManager;

protected:
	// Combo Attack Section
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UGSComboActionData> ComboAttackData;

	int32 CurrentCombo = 0;

	bool HasNextCombo = false;

	FTimerHandle ComboTimerHandle;

	void ProcessComboActionCommand();
	void AttackMontageBegin();
	void AttackMontageEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void SetComboTimer();
	void ComboCheck();
};
