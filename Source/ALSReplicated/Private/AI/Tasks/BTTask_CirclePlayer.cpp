#include "AI/Tasks/BTTask_CirclePlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"

UBTTask_CirclePlayer::UBTTask_CirclePlayer()
{
    NodeName = TEXT("Circle Player");
    Radius = 600.f;
}

EBTNodeResult::Type UBTTask_CirclePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* Controller = OwnerComp.GetAIOwner();
    if (!Controller)
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

    FVector Direction = (Pawn->GetActorLocation() - TargetActor->GetActorLocation()).GetSafeNormal();
    FVector CircleLocation = TargetActor->GetActorLocation() + Direction.RotateAngleAxis(90.f, FVector::UpVector) * Radius;

    FNavLocation Result;
    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(Pawn->GetWorld());
    if (NavSys && NavSys->ProjectPointToNavigation(CircleLocation, Result))
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsVector(DestinationKey.SelectedKeyName, Result.Location);
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}

