#pragma once

#include "CoreMinimal.h"
#include "Camera/BaseCameraComponent.h"
#include "Engine/Scene.h"
#include "CinematicCameraComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ALSREPLICATED_API UCinematicCameraComponent : public UBaseCameraComponent
{
    GENERATED_BODY()

public:
    UCinematicCameraComponent();

    UFUNCTION(BlueprintCallable, Category="Camera")
    void SwitchShoulder();

    UFUNCTION(BlueprintCallable, Category="Camera")
    void EnterFocusMode(AActor* Target);

    UFUNCTION(BlueprintCallable, Category="Camera")
    void ExitFocusMode();


    /** Triggered when focus mode begins on Target. */
    UFUNCTION(BlueprintImplementableEvent, Category="Camera")
    void OnFocusModeStarted(AActor* Target);

    /** Called when focus mode finishes. */
    UFUNCTION(BlueprintImplementableEvent, Category="Camera")
    void OnFocusModeEnded();

protected:
    UPROPERTY(BlueprintReadWrite, Category="Focus")
    bool bFocusMode = false;

    UPROPERTY(BlueprintReadWrite, Category="Focus")
    AActor* FocusTarget = nullptr;

    virtual void UpdateOrientation(float DeltaTime) override;
};

