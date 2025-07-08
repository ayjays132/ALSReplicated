#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerAnalyticsComponent.generated.h"

/** Simple analytics component that logs gameplay sessions and events */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ALSREPLICATED_API UPlayerAnalyticsComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UPlayerAnalyticsComponent();

    /** Log a custom analytics event */
    UFUNCTION(BlueprintCallable, Category="Analytics")
    void LogEvent(const FString& EventName) const;

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    void LogSessionStarted() const;
    void LogSessionEnded() const;
};
