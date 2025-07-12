#include "UI/HUDWidget.h"
#include "AccessibilitySettings.h"
#include "GameFramework/Pawn.h"

void UHUDWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    const UAccessibilitySettings* Settings = GetDefault<UAccessibilitySettings>();
    if (Settings)
    {
        SetRenderTransformScale(FVector2D(Settings->HUDScale));
        OnAccessibilitySettingsChanged(Settings->ColorBlindPreset, Settings->SubtitleScale);
    }
}

void UHUDWidget::UpdateHealth_Implementation(float NewHealth)
{
    OnHealthChanged(NewHealth);
}

void UHUDWidget::UpdateStamina_Implementation(float NewStamina)
{
    OnStaminaChanged(NewStamina);
}

void UHUDWidget::UpdateLockOnTarget_Implementation(APawn* Target)
{
    OnLockOnTargetChanged(Target);
}

void UHUDWidget::UpdateQuest_Implementation(const FMissionProgress& Progress)
{
    OnQuestProgress(Progress);
}
