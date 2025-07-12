#include "FacialAnimationComponent.h"
#include "ControlRig.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"

UFacialAnimationComponent::UFacialAnimationComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UFacialAnimationComponent::PlayEmotion(EEmotionState Emotion, USoundBase* VoiceSound, const FString& Text)
{
    OnFacialAnimationStarted.Broadcast();
    ApplyEmotionCurves(Emotion);
    SyncMouth(VoiceSound, Text);
}

void UFacialAnimationComponent::StopEmotion()
{
    OnFacialAnimationStopped.Broadcast();
}

void UFacialAnimationComponent::ApplyEmotionCurves(EEmotionState Emotion)
{
    if (!FacialControlRig)
    {
        return;
    }
    // Placeholder: set control values based on emotion state
    // In a real project this would manipulate specific curve controls
}

void UFacialAnimationComponent::SyncMouth(USoundBase* VoiceSound, const FString& Text)
{
    if (!FacialControlRig)
    {
        return;
    }
    if (VoiceSound)
    {
        // When audio is provided, you would analyse the sound and
        // drive jaw/phoneme curves accordingly. Implementation is
        // simplified here.
        if (UAudioComponent* Audio = NewObject<UAudioComponent>(this))
        {
            Audio->bAutoDestroy = true;
            Audio->SetSound(VoiceSound);
            Audio->Play();
        }
    }
    else if (!Text.IsEmpty())
    {
        // Basic text driven mouth movement could be handled here.
    }
}

