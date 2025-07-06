#include "Camera/BaseCameraComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"

UBaseCameraComponent::UBaseCameraComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    TargetArmLength = 300.f;
    DesiredArmLength = TargetArmLength;
}

void UBaseCameraComponent::ToggleShoulder()
{
    bRightShoulder = !bRightShoulder;
    OnShoulderSwitched(bRightShoulder);
}

void UBaseCameraComponent::SetDesiredArmLength(float Length)
{
    DesiredArmLength = Length;
    OnZoomChanged(Length);
}

void UBaseCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
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

void UBaseCameraComponent::UpdateOrientation(float DeltaTime)
{
}

void UBaseCameraComponent::UpdatePostProcess()
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

