#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DirectionalLight.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Net/UnrealNetwork.h"
#include "AtmosphereManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeatherStateChanged, bool, bNewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTimeOfDayChanged, float, NewTime);

/**
 * Simple manager for time of day and weather effects.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ALSREPLICATED_API UAtmosphereManager : public UActorComponent
{
    GENERATED_BODY()
public:
    UAtmosphereManager();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    /** Directional light controlled by this manager */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Atmosphere")
    ADirectionalLight* DirectionalLight = nullptr;

    /** Sky material instance that receives the time of day parameter */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Atmosphere")
    UMaterialInstanceDynamic* SkyMaterial = nullptr;

    /** Parameter name on the sky material for the time of day value */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Atmosphere")
    FName SkyTimeParameterName = TEXT("TimeOfDay");

    /** Time of day in hours (0-24) */
    UPROPERTY(ReplicatedUsing=OnRep_TimeOfDay, BlueprintReadOnly, Category="Atmosphere")
    float TimeOfDay = 12.f;

    /** Whether rain is active */
    UPROPERTY(ReplicatedUsing=OnRep_Rain, BlueprintReadOnly, Category="Weather")
    bool bRaining = false;

    /** Whether fog is active */
    UPROPERTY(ReplicatedUsing=OnRep_Fog, BlueprintReadOnly, Category="Weather")
    bool bFoggy = false;

    UFUNCTION(BlueprintCallable, Category="Atmosphere")
    void SetTimeOfDay(float NewTime);

    UFUNCTION(BlueprintCallable, Category="Atmosphere")
    void SetRaining(bool bNewRaining);

    UFUNCTION(BlueprintCallable, Category="Atmosphere")
    void SetFoggy(bool bNewFoggy);

    /** Event fired when time of day changes */
    UPROPERTY(BlueprintAssignable)
    FTimeOfDayChanged OnTimeOfDayChanged;

    /** Event fired when rain state changes */
    UPROPERTY(BlueprintAssignable)
    FWeatherStateChanged OnRainStateChanged;

    /** Event fired when fog state changes */
    UPROPERTY(BlueprintAssignable)
    FWeatherStateChanged OnFogStateChanged;

protected:
    UFUNCTION()
    void OnRep_TimeOfDay();

    UFUNCTION()
    void OnRep_Rain();

    UFUNCTION()
    void OnRep_Fog();

    UFUNCTION(Server, Reliable, WithValidation)
    void ServerSetTimeOfDay(float NewTime);

    UFUNCTION(Server, Reliable, WithValidation)
    void ServerSetRaining(bool bNewRaining);

    UFUNCTION(Server, Reliable, WithValidation)
    void ServerSetFoggy(bool bNewFoggy);

    void UpdateLighting();
};

