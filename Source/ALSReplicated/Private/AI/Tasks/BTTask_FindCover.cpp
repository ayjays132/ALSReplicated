#include "AI/Tasks/BTTask_FindCover.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindCover::UBTTask_FindCover()
{
    NodeName = TEXT("Find Cover");
    SearchRadius = 1000.f;
}

EBTNodeResult::Type UBTTask_FindCover::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* Controller = OwnerComp.GetAIOwner();
    if (!Controller)
    {
        return EBTNodeResult::Failed;
    }

    APawn* Pawn = Controller->GetPawn();
    if (!Pawn)
    {
        return EBTNodeResult::Failed;
    }

    FNavLocation Result;
    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(Pawn->GetWorld());
    if (NavSys && NavSys->GetRandomReachablePointInRadius(Pawn->GetActorLocation(), SearchRadius, Result))
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsVector(LocationKey.SelectedKeyName, Result.Location);
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}

