#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class UAccessibilitySettings;

/** Simple HUD widget that scales based on saved settings */
UCLASS(BlueprintType)
class ALSREPLICATED_API UHUDWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    virtual void NativeOnInitialized() override;
};
