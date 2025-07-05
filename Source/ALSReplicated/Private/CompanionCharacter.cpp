#include "CompanionCharacter.h"
#include "AI/ALSAIController.h"
#include "CombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

ACompanionCharacter::ACompanionCharacter(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    AIControllerClass = AALSAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ACompanionCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void ACompanionCharacter::SetFollowTarget(AActor* Target)
{
    FollowActor = Target;
    FollowTarget();

    if (FollowTimerHandle.IsValid())
    {
        GetWorldTimerManager().ClearTimer(FollowTimerHandle);
    }

    GetWorldTimerManager().SetTimer(FollowTimerHandle, this, &ACompanionCharacter::FollowTarget, 1.0f, true);
}

void ACompanionCharacter::FollowTarget()
{
    if (!FollowActor)
    {
        return;
    }

    if (AAIController* AC = Cast<AAIController>(GetController()))
    {
        AC->MoveToActor(FollowActor, 100.f);
    }
}

void ACompanionCharacter::AssistAttack()
{
    if (CombatComponent)
    {
        CombatComponent->LightAttack();
    }
}


