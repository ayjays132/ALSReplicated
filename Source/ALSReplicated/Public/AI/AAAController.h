#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "AAAController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;
class UEmotionStateComponent;

/**
 * Basic AI controller used for replicated AI characters.
 */
UCLASS()
class ALSREPLICATED_API AALSBaseAIController : public AAIController
{
    GENERATED_BODY()

public:
    AALSBaseAIController();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    virtual void OnUnPossess() override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
    UAIPerceptionComponent* PerceptionComponent;

    UPROPERTY()
    UAISenseConfig_Sight* SightConfig;

    UPROPERTY()
    UAISenseConfig_Hearing* HearingConfig;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
    UEmotionStateComponent* EmotionComponent;

    UFUNCTION()
    void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

    UFUNCTION()
    void HandleHardImpact(AActor* Instigator, FVector Location);

    UPROPERTY(ReplicatedUsing=OnRep_CurrentTarget)
    AActor* CurrentTarget;

    UFUNCTION()
    void OnRep_CurrentTarget();
};

