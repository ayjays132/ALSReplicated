#include "Camera/CinematicCameraComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"

UCinematicCameraComponent::UCinematicCameraComponent()
    : Super()
{
}

void UCinematicCameraComponent::SwitchShoulder()
{
    ToggleShoulder();
}


void UCinematicCameraComponent::EnterFocusMode(AActor* Target)
{
    FocusTarget = Target;
    bFocusMode = FocusTarget != nullptr;
    if (bFocusMode)
    {
        OnFocusModeStarted(FocusTarget);
    }
}

void UCinematicCameraComponent::ExitFocusMode()
{
    FocusTarget = nullptr;
    if (bFocusMode)
    {
        bFocusMode = false;
        OnFocusModeEnded();
    }
}

void UCinematicCameraComponent::UpdateOrientation(float DeltaTime)
{
    if (bFocusMode && FocusTarget)
    {
        FVector Direction = FocusTarget->GetActorLocation() - GetComponentLocation();
        FRotator DesiredRot = Direction.Rotation();
        SetWorldRotation(FMath::RInterpTo(GetComponentRotation(), DesiredRot, DeltaTime, LagSpeed));
    }
}
