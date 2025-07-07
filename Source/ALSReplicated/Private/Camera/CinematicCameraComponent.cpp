#include "Camera/CinematicCameraComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "CharacterStateCoordinator.h"
#include "StaminaComponent.h"

UCinematicCameraComponent::UCinematicCameraComponent()
    : Super()
{
    CurrentFOV = DefaultFOV;
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

    float TargetFOV = DefaultFOV;
    AActor* OwnerActor = GetOwner();
    if (OwnerActor)
    {
        if (UCharacterStateCoordinator* State = OwnerActor->FindComponentByClass<UCharacterStateCoordinator>())
        {
            if (State->GetCharacterState() == ECharacterActivityState::Combat)
            {
                TargetFOV = CombatFOV;
            }
        }

        if (UStaminaComponent* Stamina = OwnerActor->FindComponentByClass<UStaminaComponent>())
        {
            float Ratio = (Stamina->MaxStamina > 0.f) ? (Stamina->Stamina / Stamina->MaxStamina) : 1.f;
            if (LowStaminaSwayCurve)
            {
                TargetFOV += LowStaminaSwayCurve->GetFloatValue(1.f - Ratio);
            }
        }
    }

    float NewFOV = FMath::FInterpTo(CurrentFOV, TargetFOV, DeltaTime, 5.f);
    if (!FMath::IsNearlyEqual(NewFOV, CurrentFOV))
    {
        CurrentFOV = NewFOV;
        OnFOVChanged.Broadcast(NewFOV);

        if (OwnerActor)
        {
            APlayerController* PC = nullptr;
            if (APawn* OwnerPawn = Cast<APawn>(OwnerActor))
            {
                PC = Cast<APlayerController>(OwnerPawn->GetController());
            }
            if (!PC)
            {
                PC = Cast<APlayerController>(OwnerActor->GetInstigatorController());
            }
            if (PC && PC->PlayerCameraManager)
            {
                PC->PlayerCameraManager->SetFOV(NewFOV);
            }
        }
    }
}
