#pragma once

#include "UObject/Interface.h"
#include "MissionManagerComponent.h"
#include "HUDUpdateInterface.generated.h"

UINTERFACE(BlueprintType)
class ALSREPLICATED_API UHUDUpdateInterface : public UInterface
{
    GENERATED_BODY()
};

class ALSREPLICATED_API IHUDUpdateInterface
{
    GENERATED_BODY()
public:
    /** Update current health value */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="HUD")
    void UpdateHealth(float NewHealth);

    /** Update current stamina value */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="HUD")
    void UpdateStamina(float NewStamina);

    /** Set the current lock-on target (or null to clear) */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="HUD")
    void UpdateLockOnTarget(APawn* Target);

    /** Update quest progress */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="HUD")
    void UpdateQuest(const FMissionProgress& Progress);
};

