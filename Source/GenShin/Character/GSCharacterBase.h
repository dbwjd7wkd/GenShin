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

	// character control section
protected:
	virtual void SetCharacterControlData(const class UGSCharacterControlData* CharacterControlData);

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, TObjectPtr<class UGSCharacterControlData>> CharacterControlManager;
};
