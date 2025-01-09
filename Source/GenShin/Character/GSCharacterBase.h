// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/GSAnimationAttackInterface.h"
#include "Interface/GSCharacterWidgetInterface.h"
#include "Interface/GSCharacterItemInterface.h"
#include "GSCharacterBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGSCharacter, Log, All);

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};

DECLARE_DELEGATE_OneParam(FOnTakeItemDelegate, class UGSItemData* /*InItemData*/);
USTRUCT(BlueprintType)
struct FTakeItemDelegateWrapper
{
	GENERATED_BODY()
	FTakeItemDelegateWrapper() {}
	FTakeItemDelegateWrapper(const FOnTakeItemDelegate& InItemDelegate) : ItemDelegate(InItemDelegate) {}
	FOnTakeItemDelegate ItemDelegate;
};

UCLASS()
class GENSHIN_API AGSCharacterBase : public ACharacter, public IGSAnimationAttackInterface, public IGSCharacterWidgetInterface, public IGSCharacterItemInterface
{
	GENERATED_BODY()

public:
	AGSCharacterBase();

	virtual void PostInitializeComponents() override;

	// Character Control Section
protected:
	virtual void SetCharacterControlData(const class UGSCharacterControlData* CharacterControlData);

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, TObjectPtr<class UGSCharacterControlData>> CharacterControlManager;

	// Combo Attack Section
protected:
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

	// Attack Hit Section
protected:
	virtual void AttackHitCheck() override;
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// Dead Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> DeadMontage;

	virtual void SetDead();
	void PlayDeadAnimation();

	float DeadEventDelayTime = 5.0f;

	// Stat Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UGSCharacterStatComponent> Stat;

	// UI Widget Section
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetComponent> HpBar;

	virtual void SetupCharacterWidget(class UGSUserWidget* InUserWidget) override;

	// Item Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Weapon;

	UPROPERTY();
	TArray<FTakeItemDelegateWrapper> TakeItemActions;

	virtual void TakeItem(class UGSItemData* InItemData) override;
	void DoNothing(class UGSItemData* InItemData);
	void EquipWeapon(class UGSItemData* InItemData);
	void DrinkPotion(class UGSItemData* InItemData);
	void ReadScroll(class UGSItemData* InItemData);

};
