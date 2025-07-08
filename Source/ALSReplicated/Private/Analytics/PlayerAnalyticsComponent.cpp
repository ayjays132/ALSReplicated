#include "Analytics/PlayerAnalyticsComponent.h"
#include "GameFramework/Actor.h"
#include "ALSLog.h"

UPlayerAnalyticsComponent::UPlayerAnalyticsComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UPlayerAnalyticsComponent::BeginPlay()
{
    Super::BeginPlay();
    LogSessionStarted();
}

void UPlayerAnalyticsComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    LogSessionEnded();
    Super::EndPlay(EndPlayReason);
}

void UPlayerAnalyticsComponent::LogEvent(const FString& EventName) const
{
    UE_LOG(LogALSReplicated, Log, TEXT("Analytics Event: %s from %s"), *EventName, *GetOwner()->GetName());
}

void UPlayerAnalyticsComponent::LogSessionStarted() const
{
    UE_LOG(LogALSReplicated, Log, TEXT("Analytics Session Started for %s"), *GetOwner()->GetName());
}

void UPlayerAnalyticsComponent::LogSessionEnded() const
{
    UE_LOG(LogALSReplicated, Log, TEXT("Analytics Session Ended for %s"), *GetOwner()->GetName());
}
