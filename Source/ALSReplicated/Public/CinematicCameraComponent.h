#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/Scene.h"
#include "CinematicCameraComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ALSREPLICATED_API UCinematicCameraComponent : public USpringArmComponent
{
    GENERATED_BODY()

public:
    UCinematicCameraComponent();

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, Category="Camera")
    void SwitchShoulder();

    UFUNCTION(BlueprintCallable, Category="Camera")
    void SetDesiredArmLength(float Length);

    UFUNCTION(BlueprintCallable, Category="Camera")
    void EnterFocusMode(AActor* Target);

    UFUNCTION(BlueprintCallable, Category="Camera")
    void ExitFocusMode();

    UFUNCTION(BlueprintImplementableEvent, Category="Camera")
    void OnShoulderSwitched(bool bRight);

    UFUNCTION(BlueprintImplementableEvent, Category="Camera")
    void OnZoomChanged(float NewLength);

    UFUNCTION(BlueprintImplementableEvent, Category="Camera")
    void OnFocusModeStarted(AActor* Target);

    UFUNCTION(BlueprintImplementableEvent, Category="Camera")
    void OnFocusModeEnded();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shoulder")
    float ShoulderOffset = 50.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shoulder")
    bool bRightShoulder = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Lag")
    bool bEnableLag = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Lag", meta=(EditCondition="bEnableLag"))
    float LagSpeed = 10.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Zoom")
    float ZoomInterpSpeed = 5.f;

    UPROPERTY(BlueprintReadWrite, Category="Zoom")
    float DesiredArmLength = 300.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PostProcess")
    bool bUsePostProcess = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PostProcess", meta=(EditCondition="bUsePostProcess"))
    FPostProcessSettings PostProcessSettings;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PostProcess", meta=(EditCondition="bUsePostProcess"))
    float PostProcessBlendWeight = 1.f;

    UPROPERTY(BlueprintReadWrite, Category="Focus")
    bool bFocusMode = false;

    UPROPERTY(BlueprintReadWrite, Category="Focus")
    AActor* FocusTarget = nullptr;
};

