#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/SaveGame.h"
#include "MissionManagerComponent.h"
#include "SaveGameComponent.generated.h"

/** Save data container for player stats and world state */
UCLASS()
class ALSREPLICATED_API UPlayerSaveGame : public USaveGame
{
    GENERATED_BODY()
public:
    UPROPERTY()
    float Stamina = 100.f;

    UPROPERTY()
    float MaxStamina = 100.f;

    UPROPERTY()
    TArray<FMissionProgress> Missions;

    UPROPERTY()
    float TimeOfDay = 12.f;

    UPROPERTY()
    bool bRaining = false;

    UPROPERTY()
    bool bFoggy = false;
};

/** Component that handles saving and loading player progress */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ALSREPLICATED_API USaveGameComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    USaveGameComponent();

    UFUNCTION(BlueprintCallable, Category="Save")
    void SaveGame();

    UFUNCTION(BlueprintCallable, Category="Save")
    void LoadGame();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Save")
    FString SlotName = TEXT("PlayerSave");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Save")
    uint32 UserIndex = 0;

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void HandleMissionUpdate(const FMissionProgress& Progress);

    UFUNCTION()
    void Autosave();

    void GatherSaveData(UPlayerSaveGame* Save) const;
    void ApplySaveData(UPlayerSaveGame* Save) const;

    UPROPERTY()
    UMissionManagerComponent* MissionManager = nullptr;
};

