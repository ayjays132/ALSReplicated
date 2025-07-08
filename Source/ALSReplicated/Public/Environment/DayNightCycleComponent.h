#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "DayNightCycleComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTimeOfDayChangedSignature, float, NewTime);

/**
 * Handles replicated time of day for the environment.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ALSREPLICATED_API UDayNightCycleComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UDayNightCycleComponent();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    /** Current time of day in hours (0-24) */
    UPROPERTY(ReplicatedUsing=OnRep_TimeOfDay, BlueprintReadOnly, Category="DayNight")
    float TimeOfDay = 12.f;

    UFUNCTION(BlueprintCallable, Category="DayNight")
    void SetTimeOfDay(float NewTime);

    /** Broadcast when TimeOfDay changes */
    UPROPERTY(BlueprintAssignable)
    FTimeOfDayChangedSignature OnTimeOfDayChanged;

protected:
    UFUNCTION()
    void OnRep_TimeOfDay();

    UFUNCTION(Server, Reliable, WithValidation)
    void ServerSetTimeOfDay(float NewTime);
};
