#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MusicManager.generated.h"

class UAudioComponent;
class UCharacterStateCoordinator;
class USoundBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ALSREPLICATED_API UMusicManager : public UActorComponent
{
    GENERATED_BODY()
public:
    UMusicManager();

    /** Adjusts the volume of the intensity layer (0-1). */
    UFUNCTION(BlueprintCallable, Category="Music")
    void SetIntensity(float NewIntensity);

    /** Plays a short stinger over the current music. */
    UFUNCTION(BlueprintCallable, Category="Music")
    void PlayStinger(USoundBase* Stinger);

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void HandleStateChanged(ECharacterActivityState NewState);

    void StartMusic(USoundBase* Base, USoundBase* Intense);

    UPROPERTY(EditDefaultsOnly, Category="Music")
    USoundBase* ExplorationBase;

    UPROPERTY(EditDefaultsOnly, Category="Music")
    USoundBase* ExplorationIntensity;

    UPROPERTY(EditDefaultsOnly, Category="Music")
    USoundBase* CombatBase;

    UPROPERTY(EditDefaultsOnly, Category="Music")
    USoundBase* CombatIntensity;

    UPROPERTY(EditDefaultsOnly, Category="Music")
    float FadeDuration = 1.0f;

    UPROPERTY()
    UAudioComponent* BaseComponent = nullptr;

    UPROPERTY()
    UAudioComponent* IntensityComponent = nullptr;

    UPROPERTY()
    UCharacterStateCoordinator* StateCoordinator = nullptr;

    float Intensity = 0.f;
};

