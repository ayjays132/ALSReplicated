#include "AI/Tasks/BTTask_GetPatrolPoint.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_GetPatrolPoint::UBTTask_GetPatrolPoint()
{
    NodeName = TEXT("Get Patrol Point");
    SearchRadius = 800.f;
}

EBTNodeResult::Type UBTTask_GetPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* Controller = OwnerComp.GetAIOwner();
    if (!Controller || !Controller->HasAuthority())
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
