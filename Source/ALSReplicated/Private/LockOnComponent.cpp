#include "LockOnComponent.h"
#include "GameFramework/Actor.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/PlayerController.h"

ULockOnComponent::ULockOnComponent()
{
        PrimaryComponentTick.bCanEverTick = false;
        SetIsReplicatedByDefault(true);
}

void ULockOnComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
        Super::GetLifetimeReplicatedProps(OutLifetimeProps);
        DOREPLIFETIME(ULockOnComponent, bIsLockedOn);
        DOREPLIFETIME(ULockOnComponent, LockedActor);
}

void ULockOnComponent::ToggleLockOn()
{
        if (GetOwnerRole() < ROLE_Authority)
        {
                ServerToggleLockOn();
                return;
        }

        DoToggleLockOn();
}

void ULockOnComponent::ServerToggleLockOn_Implementation()
{
        DoToggleLockOn();
}

void ULockOnComponent::DoToggleLockOn()
{
        if (bIsLockedOn)
        {
                bIsLockedOn = false;
                LockedActor = nullptr;
                return;
        }

        PerformLineTrace();
}

void ULockOnComponent::PerformLineTrace()
{
        AActor* OwnerActor = GetOwner();
        if (!OwnerActor)
        {
                return;
        }

        APlayerController* PC = Cast<APlayerController>(OwnerActor->GetInstigatorController());
        if (!PC)
        {
                return;
        }

        FVector ViewLocation;
        FRotator ViewRotation;
        PC->GetPlayerViewPoint(ViewLocation, ViewRotation);

        const FVector End = ViewLocation + ViewRotation.Vector() * 10000.0f;

        FHitResult Hit;
        FCollisionQueryParams Params;
        Params.AddIgnoredActor(OwnerActor);

        if (GetWorld()->LineTraceSingleByChannel(Hit, ViewLocation, End, ECC_Pawn, Params))
        {
                AActor* HitActor = Hit.GetActor();
                if (HitActor && HitActor != OwnerActor)
                {
                        LockedActor = HitActor;
                        bIsLockedOn = true;
                }
        }
}

