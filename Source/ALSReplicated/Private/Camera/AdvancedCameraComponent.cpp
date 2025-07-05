#include "Camera/AdvancedCameraComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"

UAdvancedCameraComponent::UAdvancedCameraComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    TargetArmLength = 300.f;
    DesiredArmLength = TargetArmLength;
}

void UAdvancedCameraComponent::ToggleShoulder()
{
    bRightShoulder = !bRightShoulder;
    OnShoulderSwitched(bRightShoulder);
}

void UAdvancedCameraComponent::SetZoomLevel(float Length)
{
    DesiredArmLength = Length;
    OnZoomChanged(Length);
}

void UAdvancedCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
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

    APawn* OwnerPawn = Cast<APawn>(GetOwner());
    if (OwnerPawn)
    {
        ULockOnComponent* LockOn = OwnerPawn->FindComponentByClass<ULockOnComponent>();
        if (LockOn)
        {
            APawn* Target = LockOn->GetCurrentTarget();
            if (Target)
            {
                FVector Dir = Target->GetActorLocation() - GetComponentLocation();
                FRotator DesiredRot = Dir.Rotation();
                SetWorldRotation(FMath::RInterpTo(GetComponentRotation(), DesiredRot, DeltaTime, LagSpeed));
            }
        }
    }

    UpdatePostProcess();
}

void UAdvancedCameraComponent::UpdatePostProcess()
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
        APawn* OwnerPawn = Cast<APawn>(GetOwner());
        if (OwnerPawn)
        {
            if (APlayerController* PC = Cast<APlayerController>(OwnerPawn->GetController()))
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

