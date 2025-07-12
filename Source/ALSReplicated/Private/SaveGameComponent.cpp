#include "SaveGameComponent.h"
#include "Kismet/GameplayStatics.h"
#include "StaminaComponent.h"
#include "AtmosphereManager.h"
#include "CharacterStateCoordinator.h"
#include "GameFramework/Actor.h"
#include "EngineUtils.h"

USaveGameComponent::USaveGameComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USaveGameComponent::BeginPlay()
{
    Super::BeginPlay();

    MissionManager = GetOwner()->FindComponentByClass<UMissionManagerComponent>();
    if (MissionManager)
    {
        MissionManager->OnMissionUpdated.AddDynamic(this, &USaveGameComponent::HandleMissionUpdate);
    }

    if (UCharacterStateCoordinator* Coord = GetOwner()->FindComponentByClass<UCharacterStateCoordinator>())
    {
        Coord->OnTraversalAction.AddDynamic(this, &USaveGameComponent::Autosave);
    }
}

void USaveGameComponent::SaveGame()
{
    UPlayerSaveGame* Save = Cast<UPlayerSaveGame>(UGameplayStatics::CreateSaveGameObject(UPlayerSaveGame::StaticClass()));
    if (!Save)
    {
        return;
    }
    GatherSaveData(Save);
    UGameplayStatics::SaveGameToSlot(Save, SlotName, UserIndex);
}

void USaveGameComponent::LoadGame()
{
    if (USaveGame* Loaded = UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex))
    {
        if (UPlayerSaveGame* Save = Cast<UPlayerSaveGame>(Loaded))
        {
            ApplySaveData(Save);
        }
    }
}

void USaveGameComponent::HandleMissionUpdate(const FMissionProgress& Progress)
{
    if (Progress.bCompleted)
    {
        Autosave();
    }
}

void USaveGameComponent::Autosave()
{
    SaveGame();
}

void USaveGameComponent::GatherSaveData(UPlayerSaveGame* Save) const
{
    if (!Save)
    {
        return;
    }

    if (UStaminaComponent* Stamina = GetOwner()->FindComponentByClass<UStaminaComponent>())
    {
        Save->Stamina = Stamina->Stamina;
        Save->MaxStamina = Stamina->MaxStamina;
    }

    if (MissionManager)
    {
        Save->Missions = MissionManager->MissionProgress;
    }

    UAtmosphereManager* Atmos = GetOwner()->FindComponentByClass<UAtmosphereManager>();
    if (!Atmos)
    {
        for (TActorIterator<AActor> It(GetWorld()); It && !Atmos; ++It)
        {
            Atmos = (*It)->FindComponentByClass<UAtmosphereManager>();
        }
    }
    if (Atmos)
    {
        Save->TimeOfDay = Atmos->TimeOfDay;
        Save->bRaining = Atmos->bRaining;
        Save->bFoggy = Atmos->bFoggy;
    }
}

void USaveGameComponent::ApplySaveData(UPlayerSaveGame* Save) const
{
    if (!Save)
    {
        return;
    }

    if (UStaminaComponent* Stamina = GetOwner()->FindComponentByClass<UStaminaComponent>())
    {
        Stamina->SetMaxStamina(Save->MaxStamina);
        Stamina->Stamina = FMath::Clamp(Save->Stamina, 0.f, Save->MaxStamina);
    }

    if (MissionManager)
    {
        MissionManager->MissionProgress = Save->Missions;
        MissionManager->OnRep_MissionProgress();
    }

    UAtmosphereManager* Atmos = GetOwner()->FindComponentByClass<UAtmosphereManager>();
    if (!Atmos)
    {
        for (TActorIterator<AActor> It(GetWorld()); It && !Atmos; ++It)
        {
            Atmos = (*It)->FindComponentByClass<UAtmosphereManager>();
        }
    }
    if (Atmos)
    {
        Atmos->SetTimeOfDay(Save->TimeOfDay);
        Atmos->SetRaining(Save->bRaining);
        Atmos->SetFoggy(Save->bFoggy);
    }
}

