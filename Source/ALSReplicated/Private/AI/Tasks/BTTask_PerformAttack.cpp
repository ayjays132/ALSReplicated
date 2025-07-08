#include "AI/Tasks/BTTask_PerformAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ALSBaseCharacter.h"
#include "CombatComponent.h"

UBTTask_PerformAttack::UBTTask_PerformAttack()
{
    NodeName = LOCTEXT("BTTask_PerformAttack_Name", "Perform Attack");
}

EBTNodeResult::Type UBTTask_PerformAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* Controller = OwnerComp.GetAIOwner();
    if (!Controller || !Controller->HasAuthority())
    {
        return EBTNodeResult::Failed;
    }

    APawn* Pawn = Controller->GetPawn();
    if (AALSBaseCharacter* ALSChar = Cast<AALSBaseCharacter>(Pawn))
    {
        if (UCombatComponent* Combat = ALSChar->FindComponentByClass<UCombatComponent>())
        {
            Combat->LightAttack();
            return EBTNodeResult::Succeeded;
        }
    }

    return EBTNodeResult::Failed;
}
