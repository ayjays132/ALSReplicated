#include "LockOnComponent.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Engine/Engine.h"
#include "EngineUtils.h"

ULockOnComponent::ULockOnComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetComponentTickEnabled(false);
    SetIsReplicatedByDefault(true);
}

void ULockOnComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ULockOnComponent, LockedTargetId);
    DOREPLIFETIME(ULockOnComponent, bTickActive);
}

void ULockOnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (CurrentTarget && !HasLineOfSight(CurrentTarget))
    {
        DoToggleLockOn();
    }
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
    if (CurrentTarget)
    {
        bTickActive = false;
        SetComponentTickEnabled(false);
        CurrentTarget = nullptr;
        LockedTargetId = FNetworkGUID();
        HideReticle();
        return;
    }

    APawn* NewTarget = FindNearestTarget();
    if (NewTarget)
    {
        bTickActive = true;
        SetComponentTickEnabled(true);
        CurrentTarget = NewTarget;

        if (UNetDriver* Driver = GetWorld()->GetNetDriver())
        {
            LockedTargetId = Driver->GuidCache->GetOrAssignNetGUID(NewTarget);
        }

        ShowReticle(NewTarget);
    }
}

APawn* ULockOnComponent::FindNearestTarget() const
{
    AActor* OwnerActor = GetOwner();
    if (!OwnerActor)
    {
        return nullptr;
    }

    APlayerController* PC = Cast<APlayerController>(OwnerActor->GetInstigatorController());
    if (!PC)
    {
        return nullptr;
    }

    FVector ViewLocation;
    FRotator ViewRotation;
    PC->GetPlayerViewPoint(ViewLocation, ViewRotation);

    APawn* ClosestPawn = nullptr;
    float ClosestDistSq = TraceRange * TraceRange;

    for (TActorIterator<APawn> It(GetWorld()); It; ++It)
    {
        APawn* Pawn = *It;
        if (!Pawn || Pawn == OwnerActor)
        {
            continue;
        }

        float DistSq = FVector::DistSquared(ViewLocation, Pawn->GetActorLocation());
        if (DistSq > ClosestDistSq)
        {
            continue;
        }

        if (HasLineOfSight(Pawn))
        {
            ClosestDistSq = DistSq;
            ClosestPawn = Pawn;
        }
    }

    return ClosestPawn;
}

bool ULockOnComponent::HasLineOfSight(APawn* Target) const
{
    if (!Target)
    {
        return false;
    }

    APlayerController* PC = Cast<APlayerController>(GetOwner()->GetInstigatorController());
    if (!PC)
    {
        return false;
    }

    FVector ViewLocation;
    FRotator ViewRotation;
    PC->GetPlayerViewPoint(ViewLocation, ViewRotation);

    FHitResult Hit;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(LockOn), false, GetOwner());
    Params.AddIgnoredActor(Target);

    bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, ViewLocation, Target->GetActorLocation(), ECC_Visibility, Params);

    return !bHit || Hit.GetActor() == Target;
}

void ULockOnComponent::OnRep_LockedTarget()
{
    if (UNetDriver* Driver = GetWorld()->GetNetDriver())
    {
        CurrentTarget = Cast<APawn>(Driver->GuidCache->GetObjectFromNetGUID(LockedTargetId));
    }

    if (CurrentTarget)
    {
        ShowReticle(CurrentTarget);
    }
    else
    {
        HideReticle();
    }

    SetComponentTickEnabled(bTickActive);
}

void ULockOnComponent::OnRep_TickActive()
{
    SetComponentTickEnabled(bTickActive);
}

