#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/Scene.h"
#include "LockOnComponent.h"
#include "AdvancedCameraComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ALSREPLICATED_API UAdvancedCameraComponent : public USpringArmComponent
{
    GENERATED_BODY()

public:
    UAdvancedCameraComponent();

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, Category="Camera")
    void ToggleShoulder();

    UFUNCTION(BlueprintCallable, Category="Camera")
    void SetZoomLevel(float Length);

    /** Called after toggling shoulders. bRight indicates the new shoulder. */
    UFUNCTION(BlueprintImplementableEvent, Category="Camera")
    void OnShoulderSwitched(bool bRight);

    /** Fired when DesiredArmLength is changed to NewLength. */
    UFUNCTION(BlueprintImplementableEvent, Category="Camera")
    void OnZoomChanged(float NewLength);

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

    /** Update the cached post process blend when settings change */
    UFUNCTION(BlueprintCallable, Category="PostProcess")
    void UpdatePostProcess();

    bool bPostProcessApplied = false;
    FPostProcessSettings CachedPostProcessSettings;
    float CachedBlendWeight = 0.f;
};

