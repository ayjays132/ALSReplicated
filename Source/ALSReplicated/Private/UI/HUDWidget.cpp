#include "UI/HUDWidget.h"
#include "AccessibilitySettings.h"

void UHUDWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    const UAccessibilitySettings* Settings = GetDefault<UAccessibilitySettings>();
    if (Settings)
    {
        SetRenderTransformScale(FVector2D(Settings->HUDScale));
    }
}
