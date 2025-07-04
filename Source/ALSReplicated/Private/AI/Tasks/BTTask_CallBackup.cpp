#include "AI/Tasks/BTTask_CallBackup.h"
#include "GameFramework/Actor.h"
#include "AIController.h"

UBTTask_CallBackup::UBTTask_CallBackup()
{
    NodeName = TEXT("Call Backup");
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
        UE_LOG(LogTemp, Warning, TEXT("%s is calling for backup!"), *OwnerActor->GetName());
    }

    return EBTNodeResult::Succeeded;
}

