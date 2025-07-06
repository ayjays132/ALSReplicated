#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/Scene.h"
#include "BaseCameraComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ALSREPLICATED_API UBaseCameraComponent : public USpringArmComponent
{
    GENERATED_BODY()

public:
    UBaseCameraComponent();

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, Category="Camera")
    void ToggleShoulder();

    UFUNCTION(BlueprintCallable, Category="Camera")
    void SetDesiredArmLength(float Length);

    UFUNCTION(BlueprintImplementableEvent, Category="Camera")
    void OnShoulderSwitched(bool bRight);

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

    UFUNCTION(BlueprintCallable, Category="PostProcess")
    void UpdatePostProcess();

    bool bPostProcessApplied = false;
    FPostProcessSettings CachedPostProcessSettings;
    float CachedBlendWeight = 0.f;

    virtual void UpdateOrientation(float DeltaTime);
};

