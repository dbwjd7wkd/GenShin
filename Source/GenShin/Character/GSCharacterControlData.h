// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GSCharacterControlData.generated.h"

/**
 * 뷰전환 시 바뀌는 캐릭터 컨트롤에 관련한 옵션들을 모아둔 데이터애셋
 */
UCLASS()
class GENSHIN_API UGSCharacterControlData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UGSCharacterControlData();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	// pawn section
	UPROPERTY(EditAnywhere, Category = Pawn)
	uint32 bUseControllerRotationYaw : 1;

	// character movement section
	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	uint32 bOrientRotationToMovement : 1;

	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	uint32 bUseControllerDesiredRotation : 1;

	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	FRotator RotationRate;

	// spring arm section
	UPROPERTY(EditAnywhere, Category = SpringArm)
	float TargetArmLength;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	FRotator RelativeRotation;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bUsePawnControlRotation : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bInheritPitch : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bInheritYaw : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bInheritRoll : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bDoCollisionTest : 1;
};
