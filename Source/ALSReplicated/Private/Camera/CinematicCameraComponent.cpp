#include "Camera/CinematicCameraComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"

UCinematicCameraComponent::UCinematicCameraComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    TargetArmLength = 300.f;
    DesiredArmLength = TargetArmLength;
}

void UCinematicCameraComponent::SwitchShoulder()
{
    ToggleShoulder();
}

void UCinematicCameraComponent::SetDesiredArmLength(float Length)
{
    DesiredArmLength = Length;
    OnZoomChanged(Length);
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

void UCinematicCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    TargetArmLength = FMath::FInterpTo(TargetArmLength, DesiredArmLength, DeltaTime, ZoomInterpSpeed);

    FVector DesiredOffset(0.f, bRightShoulder ? ShoulderOffset : -ShoulderOffset, 0.f);
    if (bEnableLag)
    {
        SocketOffset = FMath::VInterpTo(SocketOffset, DesiredOffset, DeltaTime, LagSpeed);
    }
    else
    {
        SocketOffset = DesiredOffset;
    }

    UpdateOrientation(DeltaTime);

    UpdatePostProcess();
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

void UCinematicCameraComponent::UpdatePostProcess()
{
    if (!bUsePostProcess)
    {
        bPostProcessApplied = false;
        return;
    }

    const bool bSettingsChanged = !PostProcessSettings.Equals(CachedPostProcessSettings);
    const bool bWeightChanged = !FMath::IsNearlyEqual(PostProcessBlendWeight, CachedBlendWeight);

    if (!bPostProcessApplied || bSettingsChanged || bWeightChanged)
    {
        AActor* OwnerActor = GetOwner();
        APlayerController* PC = nullptr;
        if (APawn* OwnerPawn = Cast<APawn>(OwnerActor))
        {
            PC = Cast<APlayerController>(OwnerPawn->GetController());
        }
        if (!PC && OwnerActor)
        {
            PC = Cast<APlayerController>(OwnerActor->GetInstigatorController());
        }
        if (PC && PC->PlayerCameraManager)
        {
            PC->PlayerCameraManager->AddCachedPPBlend(PostProcessSettings, PostProcessBlendWeight);
            CachedPostProcessSettings = PostProcessSettings;
            CachedBlendWeight = PostProcessBlendWeight;
            bPostProcessApplied = true;
        }
    }
}

