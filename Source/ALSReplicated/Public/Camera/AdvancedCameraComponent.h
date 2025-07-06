#pragma once

#include "CoreMinimal.h"
#include "Camera/BaseCameraComponent.h"
#include "Engine/Scene.h"
#include "LockOnComponent.h"
#include "AdvancedCameraComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ALSREPLICATED_API UAdvancedCameraComponent : public UBaseCameraComponent
{
    GENERATED_BODY()

public:
    UAdvancedCameraComponent();

    UFUNCTION(BlueprintCallable, Category="Camera")
    void SetZoomLevel(float Length);

protected:
    virtual void UpdateOrientation(float DeltaTime) override;
};

