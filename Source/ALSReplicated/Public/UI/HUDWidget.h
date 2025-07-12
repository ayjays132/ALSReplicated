#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/HUDUpdateInterface.h"
#include "HUDWidget.generated.h"

class UAccessibilitySettings;

/** Simple HUD widget that scales based on saved settings */
UCLASS(BlueprintType)
class ALSREPLICATED_API UHUDWidget : public UUserWidget, public IHUDUpdateInterface
{
    GENERATED_BODY()
public:
    virtual void NativeOnInitialized() override;

    virtual void UpdateHealth_Implementation(float NewHealth) override;
    virtual void UpdateStamina_Implementation(float NewStamina) override;
    virtual void UpdateLockOnTarget_Implementation(APawn* Target) override;
    virtual void UpdateQuest_Implementation(const FMissionProgress& Progress) override;

    UFUNCTION(BlueprintImplementableEvent, Category="HUD")
    void OnHealthChanged(float NewHealth);

    UFUNCTION(BlueprintImplementableEvent, Category="HUD")
    void OnStaminaChanged(float NewStamina);

    UFUNCTION(BlueprintImplementableEvent, Category="HUD")
    void OnLockOnTargetChanged(APawn* Target);

    UFUNCTION(BlueprintImplementableEvent, Category="HUD")
    void OnQuestProgress(const FMissionProgress& Progress);

    /** Apply accessibility settings such as colorblind preset and subtitle scale */
    UFUNCTION(BlueprintImplementableEvent, Category="HUD")
    void OnAccessibilitySettingsChanged(EColorBlindPreset Preset, float SubtitleScale);
};
