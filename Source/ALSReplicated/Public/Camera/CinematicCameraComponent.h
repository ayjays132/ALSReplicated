#pragma once

#include "CoreMinimal.h"
#include "Camera/BaseCameraComponent.h"
#include "Engine/Scene.h"
#include "Curves/CurveFloat.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCameraFOVChanged, float, NewFOV);

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

    /** Broadcast whenever the camera field of view changes */
    UPROPERTY(BlueprintAssignable, Category="FOV")
    FCameraFOVChanged OnFOVChanged;


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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FOV")
    float DefaultFOV = 90.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FOV")
    float CombatFOV = 80.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FOV")
    UCurveFloat* LowStaminaSwayCurve = nullptr;

    UPROPERTY(BlueprintReadOnly, Category="FOV")
    float CurrentFOV = 90.f;

    virtual void UpdateOrientation(float DeltaTime) override;
};

