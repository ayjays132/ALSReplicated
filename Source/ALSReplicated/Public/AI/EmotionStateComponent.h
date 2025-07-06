#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EmotionStateComponent.generated.h"

UENUM(BlueprintType)
enum class EEmotionState : uint8
{
    Calm,
    Alert,
    Fear,
    Aggression,
    Confused
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ALSREPLICATED_API UEmotionStateComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UEmotionStateComponent();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION(BlueprintCallable, Category="Emotion")
    void SetEmotion(EEmotionState NewEmotion);

    UFUNCTION(BlueprintCallable, Category="Emotion")
    EEmotionState GetEmotion() const { return CurrentEmotion; }

    void HandlePerception(bool bSensedEnemy);

protected:
    UPROPERTY(ReplicatedUsing=OnRep_Emotion, EditAnywhere, BlueprintReadOnly, Category="Emotion")
    EEmotionState CurrentEmotion = EEmotionState::Calm;

    UFUNCTION()
    void OnRep_Emotion();
};
