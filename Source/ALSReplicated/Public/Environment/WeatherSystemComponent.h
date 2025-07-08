#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "WeatherSystemComponent.generated.h"

UENUM(BlueprintType)
enum class EWeatherType : uint8
{
    Clear,
    Rain,
    Snow
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeatherChangedSignature, EWeatherType, NewWeather);

/**
 * Replicated weather state for the world.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ALSREPLICATED_API UWeatherSystemComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UWeatherSystemComponent();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UPROPERTY(ReplicatedUsing=OnRep_Weather, BlueprintReadOnly, Category="Weather")
    EWeatherType CurrentWeather = EWeatherType::Clear;

    UFUNCTION(BlueprintCallable, Category="Weather")
    void SetWeather(EWeatherType NewWeather);

    UPROPERTY(BlueprintAssignable)
    FWeatherChangedSignature OnWeatherChanged;

protected:
    UFUNCTION()
    void OnRep_Weather();

    UFUNCTION(Server, Reliable, WithValidation)
    void ServerSetWeather(EWeatherType NewWeather);
};
