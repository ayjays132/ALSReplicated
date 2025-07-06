#include "Camera/AdvancedCameraComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"

UAdvancedCameraComponent::UAdvancedCameraComponent()
{
}

void UAdvancedCameraComponent::SetZoomLevel(float Length)
{
    SetDesiredArmLength(Length);
}


void UAdvancedCameraComponent::UpdateOrientation(float DeltaTime)
{
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
}

