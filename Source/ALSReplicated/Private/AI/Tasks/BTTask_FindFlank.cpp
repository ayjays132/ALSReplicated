#include "AI/Tasks/BTTask_FindFlank.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"

UBTTask_FindFlank::UBTTask_FindFlank()
{
    NodeName = LOCTEXT("BTTask_FindFlank_Name", "Find Flank Position");
    Distance = 400.f;
}

EBTNodeResult::Type UBTTask_FindFlank::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* Controller = OwnerComp.GetAIOwner();
    if (!Controller || !Controller->HasAuthority())
    {
        return EBTNodeResult::Failed;
    }

    APawn* Pawn = Controller->GetPawn();
    UObject* TargetObj = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName);
    AActor* TargetActor = Cast<AActor>(TargetObj);
    if (!Pawn || !TargetActor)
    {
        return EBTNodeResult::Failed;
    }

    FVector Direction = (TargetActor->GetActorLocation() - Pawn->GetActorLocation()).GetSafeNormal();
    FVector RightVector = FVector::CrossProduct(Direction, FVector::UpVector);
    FVector DesiredLocation = TargetActor->GetActorLocation() + RightVector * Distance;

    FNavLocation Result;
    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(Pawn->GetWorld());
    if (NavSys && NavSys->ProjectPointToNavigation(DesiredLocation, Result))
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsVector(DestinationKey.SelectedKeyName, Result.Location);
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}
