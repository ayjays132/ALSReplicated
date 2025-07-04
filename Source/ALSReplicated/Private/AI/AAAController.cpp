#include "AI/AAAController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Net/UnrealNetwork.h"

AALSBaseAIController::AALSBaseAIController()
{
    bReplicates = true;

    PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));

    if (PerceptionComponent)
    {
        PerceptionComponent->SetIsReplicated(true);
        PerceptionComponent->ConfigureSense(*SightConfig);
        PerceptionComponent->ConfigureSense(*HearingConfig);
        PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
        PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AALSBaseAIController::OnTargetPerceptionUpdated);
    }
}

void AALSBaseAIController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AALSBaseAIController, CurrentTarget);
}

void AALSBaseAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    if (Stimulus.WasSuccessfullySensed())
    {
        CurrentTarget = Actor;
        OnRep_CurrentTarget();
    }
    else if (Actor == CurrentTarget)
    {
        CurrentTarget = nullptr;
        OnRep_CurrentTarget();
    }
}

void AALSBaseAIController::OnRep_CurrentTarget()
{
    // Hook for blueprint or native code
}

