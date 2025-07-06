#include "AI/EmotionStateComponent.h"
#include "Net/UnrealNetwork.h"

UEmotionStateComponent::UEmotionStateComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

void UEmotionStateComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UEmotionStateComponent, CurrentEmotion);
}

void UEmotionStateComponent::SetEmotion(EEmotionState NewEmotion)
{
    if (GetOwnerRole() < ROLE_Authority)
    {
        return;
    }
    CurrentEmotion = NewEmotion;
    OnRep_Emotion();
}

void UEmotionStateComponent::HandlePerception(bool bSensedEnemy)
{
    if (bSensedEnemy)
    {
        SetEmotion(EEmotionState::Aggression);
    }
    else
    {
        SetEmotion(EEmotionState::Calm);
    }
}

void UEmotionStateComponent::OnRep_Emotion()
{
    // Placeholder for reactions when emotion changes
}
