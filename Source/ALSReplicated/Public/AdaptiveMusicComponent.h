#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStateCoordinator.h"
#include "AdaptiveMusicComponent.generated.h"

class UAudioComponent;
class USoundBase;

/**
 * Plays different music cues depending on the current character activity state.
 */
UCLASS(ClassGroup=(Audio), meta=(BlueprintSpawnableComponent))
class ALSREPLICATED_API UAdaptiveMusicComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UAdaptiveMusicComponent();

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void HandleStateChanged(ECharacterActivityState NewState);

    void PlayCue(USoundBase* Cue);

    UPROPERTY(EditDefaultsOnly, Category="Adaptive Music")
    USoundBase* ExplorationCue;

    UPROPERTY(EditDefaultsOnly, Category="Adaptive Music")
    USoundBase* CombatCue;

    UPROPERTY(EditDefaultsOnly, Category="Adaptive Music")
    float FadeDuration = 1.f;

    UPROPERTY()
    UAudioComponent* CurrentComponent = nullptr;

    UPROPERTY()
    UCharacterStateCoordinator* StateCoordinator = nullptr;
};

