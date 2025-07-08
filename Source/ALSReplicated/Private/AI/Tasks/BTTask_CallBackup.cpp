#include "AI/Tasks/BTTask_CallBackup.h"
#include "GameFramework/Actor.h"
#include "AIController.h"
#include "ALSLog.h"

UBTTask_CallBackup::UBTTask_CallBackup()
{
    NodeName = LOCTEXT("BTTask_CallBackup_Name", "Call Backup");
}

EBTNodeResult::Type UBTTask_CallBackup::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* Controller = OwnerComp.GetAIOwner();
    if (!Controller)
    {
        return EBTNodeResult::Failed;
    }

    if (AActor* OwnerActor = Controller->GetPawn())
    {
        UE_LOG(LogALSReplicated, Warning, TEXT("%s is calling for backup!"), *OwnerActor->GetName());
    }

    return EBTNodeResult::Succeeded;
}

