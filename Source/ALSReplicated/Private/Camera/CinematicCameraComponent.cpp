#include "CinematicCameraComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

UCinematicCameraComponent::UCinematicCameraComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    TargetArmLength = 300.f;
    DesiredArmLength = TargetArmLength;
}

void UCinematicCameraComponent::SwitchShoulder()
{
    bRightShoulder = !bRightShoulder;
    OnShoulderSwitched(bRightShoulder);
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

    if (bFocusMode && FocusTarget)
    {
        FVector Direction = FocusTarget->GetActorLocation() - GetComponentLocation();
        FRotator DesiredRot = Direction.Rotation();
        SetWorldRotation(FMath::RInterpTo(GetComponentRotation(), DesiredRot, DeltaTime, LagSpeed));
    }

    UpdatePostProcess();
}

void UCinematicCameraComponent::UpdatePostProcess()
{
    if (!bUsePostProcess)
    {
        bPostProcessApplied = false;
        return;
    }

    bool bSettingsChanged = !PostProcessSettings.Equals(CachedPostProcessSettings);
    bool bWeightChanged = !FMath::IsNearlyEqual(PostProcessBlendWeight, CachedBlendWeight);

    if (!bPostProcessApplied || bSettingsChanged || bWeightChanged)
    {
        if (AActor* OwnerActor = GetOwner())
        {
            if (APlayerController* PC = Cast<APlayerController>(OwnerActor->GetInstigatorController()))
            {
                if (APlayerCameraManager* PCM = PC->PlayerCameraManager)
                {
                    PCM->AddCachedPPBlend(PostProcessSettings, PostProcessBlendWeight);
                    CachedPostProcessSettings = PostProcessSettings;
                    CachedBlendWeight = PostProcessBlendWeight;
                    bPostProcessApplied = true;
                }
            }
        }
    }
}

