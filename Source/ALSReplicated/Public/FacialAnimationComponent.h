#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AI/EmotionStateComponent.h"
#include "FacialAnimationComponent.generated.h"

class UControlRig;
class USoundBase;

/**
 * Drives MetaHuman facial curves using a Control Rig. Provides
 * helpers for playing emotion-specific animations and syncing
 * mouth movement to spoken audio or text.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ALSREPLICATED_API UFacialAnimationComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UFacialAnimationComponent();

    /**
     * Play a facial animation for the given emotion. Optionally
     * provide voice audio or subtitle text which will be used to
     * drive mouth movement.
     */
    UFUNCTION(BlueprintCallable, Category="Facial Animation")
    void PlayEmotion(EEmotionState Emotion, USoundBase* VoiceSound, const FString& Text);

    /** Stop the current facial animation */
    UFUNCTION(BlueprintCallable, Category="Facial Animation")
    void StopEmotion();

    /** Control rig asset driving the facial curves */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Facial Animation")
    UControlRig* FacialControlRig = nullptr;

    /** Broadcast when a facial animation begins */
    UPROPERTY(BlueprintAssignable, Category="Facial Animation")
    FSimpleMulticastDelegate OnFacialAnimationStarted;

    /** Broadcast when a facial animation stops */
    UPROPERTY(BlueprintAssignable, Category="Facial Animation")
    FSimpleMulticastDelegate OnFacialAnimationStopped;

protected:
    void ApplyEmotionCurves(EEmotionState Emotion);
    void SyncMouth(USoundBase* VoiceSound, const FString& Text);
};

