#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AccessibilitySettings.generated.h"

UENUM(BlueprintType)
enum class EColorBlindPreset : uint8
{
    Normal      UMETA(DisplayName="Normal"),
    Deuteranope UMETA(DisplayName="Deuteranope"),
    Protanope   UMETA(DisplayName="Protanope"),
    Tritanope   UMETA(DisplayName="Tritanope")
};

/** Stores accessibility related user settings */
UCLASS(Config=Game, DefaultConfig, BlueprintType)
class ALSREPLICATED_API UAccessibilitySettings : public UObject
{
    GENERATED_BODY()
public:
    UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category="Accessibility")
    EColorBlindPreset ColorBlindPreset = EColorBlindPreset::Normal;

    UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category="Accessibility", meta=(ClampMin="0.5", ClampMax="3.0"))
    float SubtitleScale = 1.0f;

    UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category="Accessibility", meta=(ClampMin="0.5", ClampMax="3.0"))
    float HUDScale = 1.0f;

    UFUNCTION(BlueprintCallable, Category="Accessibility")
    static UAccessibilitySettings* Get() { return GetMutableDefault<UAccessibilitySettings>(); }

    UFUNCTION(BlueprintCallable, Category="Accessibility")
    void Save();
};
