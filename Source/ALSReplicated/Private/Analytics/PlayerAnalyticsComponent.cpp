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
    UE_LOG(LogALSReplicated, Log, TEXT("%s"), *FText::Format(
        LOCTEXT("AnalyticsEventLog", "Analytics Event: {0} from {1}"),
        FText::FromString(EventName),
        FText::FromString(GetOwner()->GetName())).ToString());
}

void UPlayerAnalyticsComponent::LogSessionStarted() const
{
    UE_LOG(LogALSReplicated, Log, TEXT("%s"), *FText::Format(
        LOCTEXT("AnalyticsSessionStarted", "Analytics Session Started for {0}"),
        FText::FromString(GetOwner()->GetName())).ToString());
}

void UPlayerAnalyticsComponent::LogSessionEnded() const
{
    UE_LOG(LogALSReplicated, Log, TEXT("%s"), *FText::Format(
        LOCTEXT("AnalyticsSessionEnded", "Analytics Session Ended for {0}"),
        FText::FromString(GetOwner()->GetName())).ToString());
}
