#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Camera/CameraShakeBase.h"
#include "Materials/MaterialInterface.h"
#include "VisualImpactSystem.generated.h"

/** Utility system for spawning impact related visuals */
UCLASS()
class ALSREPLICATED_API UVisualImpactSystem : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:
    /** Spawn a Niagara FX at the location oriented along the given direction. Will optionally trigger a camera shake if Force exceeds ShakeThreshold. */
    UFUNCTION(BlueprintCallable, Category="Impact")
    static void SpawnDirectionalNiagaraFX(UObject* WorldContextObject, UNiagaraSystem* FX, const FVector& Location, const FVector& Direction, float Force = 1.f, TSubclassOf<UCameraShakeBase> CameraShake = nullptr, float ShakeThreshold = 1.5f);

    /** Spawn a decal at the location oriented along the direction. */
    UFUNCTION(BlueprintCallable, Category="Impact")
    static void SpawnImpactDecal(UObject* WorldContextObject, UMaterialInterface* Decal, const FVector& Location, const FVector& Direction, float Force = 1.f, FVector Size = FVector(10.f,10.f,10.f), float LifeSpan = 5.f);
};

