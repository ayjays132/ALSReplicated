#include "AI/Tasks/BTTask_SetEmotion.h"
#include "AI/EmotionStateComponent.h"
#include "AIController.h"

UBTTask_SetEmotion::UBTTask_SetEmotion()
{
    NodeName = LOCTEXT("BTTask_SetEmotion_Name", "Set Emotion");
    NewEmotion = EEmotionState::Calm;
}

EBTNodeResult::Type UBTTask_SetEmotion::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* Controller = OwnerComp.GetAIOwner();
    if (!Controller)
    {
        return EBTNodeResult::Failed;
    }

    if (UEmotionStateComponent* Emotion = Controller->FindComponentByClass<UEmotionStateComponent>())
    {
        Emotion->SetEmotion(NewEmotion);
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}
