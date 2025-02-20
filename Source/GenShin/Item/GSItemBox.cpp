// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/GSItemBox.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Physics/GSCollision.h"
#include "Interface/GSCharacterItemInterface.h"
#include "Engine/AssetManager.h"
#include "Item/GSItemData.h"

// Sets default values
AGSItemBox::AGSItemBox()
{
    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect"));

    RootComponent = TriggerBox;
    Mesh->SetupAttachment(TriggerBox);
    Effect->SetupAttachment(TriggerBox);

    TriggerBox->SetCollisionProfileName(CPROFILE_GSTRIGGER);
    TriggerBox->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));
    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AGSItemBox::OnOverlapBegin);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(TEXT("/Script/Engine.StaticMesh'/Game/GenShin/Environment/Props/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1'"));
    if (MeshRef.Object)
    {
        Mesh->SetStaticMesh(MeshRef.Object);
    }
    Mesh->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));
    Mesh->SetCollisionProfileName(TEXT("NoCollision"));

    static ConstructorHelpers::FObjectFinder<UParticleSystem> EffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/GenShin/Effect/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh'"));
    if (EffectRef.Object)
    {
        Effect->SetTemplate(EffectRef.Object);
		Effect->bAutoActivate = false;
    }

    Item = CreateDefaultSubobject<UGSItemData>(TEXT("ItemData"));

}

void AGSItemBox::PostInitializeComponents()
{
    // "GSItemData" 태그를 가진 에셋 중 아이템 하나 로딩하기
    Super::PostInitializeComponents();

    UAssetManager& Manager = UAssetManager::Get();

    TArray<FPrimaryAssetId> Assets;
    Manager.GetPrimaryAssetIdList(TEXT("GSItemData"), Assets);
    
    ensure(0 < Assets.Num());
    int32 RandomIndex = FMath::RandRange(0, Assets.Num() - 1);
    FSoftObjectPtr AssetPtr(Manager.GetPrimaryAssetPath(Assets[RandomIndex]));
    if (AssetPtr.IsPending())
    {
        AssetPtr.LoadSynchronous();
    }
    Item = Cast<UGSItemData>(AssetPtr.Get());
    ensure(Item);
}

void AGSItemBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
    IGSCharacterItemInterface* OverlappingCharacter = Cast<IGSCharacterItemInterface>(OtherActor);
    if (OverlappingCharacter)
    {
        OverlappingCharacter->TakeItem(Item);
    }

    SetActorEnableCollision(false);
    Mesh->SetHiddenInGame(true);
    Effect->Activate(true);
    Effect->OnSystemFinished.AddDynamic(this, &AGSItemBox::OnEffectFinished);
}

void AGSItemBox::OnEffectFinished(UParticleSystemComponent* PSystem)
{
    Destroy();
}

