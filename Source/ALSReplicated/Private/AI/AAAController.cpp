#include "AI/AAAController.h"
#include "AI/EmotionStateComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Net/UnrealNetwork.h"
#include "ImpactEventSubsystem.h"

AALSBaseAIController::AALSBaseAIController()
{
    bReplicates = true;

    PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
    EmotionComponent = CreateDefaultSubobject<UEmotionStateComponent>(TEXT("EmotionComponent"));

    if (PerceptionComponent)
    {
        PerceptionComponent->SetIsReplicated(true);
        PerceptionComponent->ConfigureSense(*SightConfig);
        PerceptionComponent->ConfigureSense(*HearingConfig);
        PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
        PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AALSBaseAIController::OnTargetPerceptionUpdated);
    }
}

void AALSBaseAIController::BeginPlay()
{
    Super::BeginPlay();

    if (UWorld* World = GetWorld())
    {
        if (UImpactEventSubsystem* Subsystem = World->GetSubsystem<UImpactEventSubsystem>())
        {
            Subsystem->OnHardImpact.AddDynamic(this, &AALSBaseAIController::HandleHardImpact);
        }
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

    if (EmotionComponent)
    {
        EmotionComponent->HandlePerception(Stimulus.WasSuccessfullySensed());
        if (UBlackboardComponent* BB = GetBlackboardComponent())
        {
            BB->SetValueAsEnum(TEXT("Emotion"), (uint8)EmotionComponent->GetEmotion());
        }
    }
}

void AALSBaseAIController::OnRep_CurrentTarget()
{
    // Hook for blueprint or native code
}

void AALSBaseAIController::HandleHardImpact(AActor* Instigator, FVector Location)
{
    APawn* Controlled = GetPawn();
    if (!Controlled)
    {
        return;
    }

    const float DistSq = FVector::DistSquared(Controlled->GetActorLocation(), Location);
    if (DistSq <= FMath::Square(1000.f) && EmotionComponent)
    {
        EmotionComponent->SetEmotion(EEmotionState::Alert);
    }
}

void AALSBaseAIController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (UWorld* World = GetWorld())
    {
        if (UImpactEventSubsystem* Subsystem = World->GetSubsystem<UImpactEventSubsystem>())
        {
            Subsystem->OnHardImpact.RemoveDynamic(this, &AALSBaseAIController::HandleHardImpact);
        }
    }
    Super::EndPlay(EndPlayReason);
}

void AALSBaseAIController::OnUnPossess()
{
    if (UWorld* World = GetWorld())
    {
        if (UImpactEventSubsystem* Subsystem = World->GetSubsystem<UImpactEventSubsystem>())
        {
            Subsystem->OnHardImpact.RemoveDynamic(this, &AALSBaseAIController::HandleHardImpact);
        }
    }

    Super::OnUnPossess();
}

