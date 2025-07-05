#include "AI/ALSAIController.h"
#include "BehaviorTree/BehaviorTree.h"

AALSAIController::AALSAIController()
{
}

void AALSAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (BehaviorTreeAsset && HasAuthority())
    {
        RunBehaviorTree(BehaviorTreeAsset);
    }
}
