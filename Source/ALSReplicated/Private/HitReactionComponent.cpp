#include "HitReactionComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "VisualImpactSystem.h"
#include "Camera/CameraShakeBase.h"
#include "Net/UnrealNetwork.h"
#include "StaminaComponent.h"
#include "ImpactEventSubsystem.h"

UHitReactionComponent::UHitReactionComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

void UHitReactionComponent::BeginPlay()
{
    Super::BeginPlay();

    if (AActor* Owner = GetOwner())
    {
        StaminaComponent = Owner->FindComponentByClass<UStaminaComponent>();
    }
}

void UHitReactionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UHitReactionComponent, bIsKnockedOut);
    DOREPLIFETIME(UHitReactionComponent, bIsRagdoll);
}

void UHitReactionComponent::ReceiveHit(FVector HitLocation, FVector HitDirection, bool bCriticalHit, bool bBlocked, bool bKnockout)
{
    if (GetOwnerRole() < ROLE_Authority)
    {
        ServerReceiveHit(HitLocation, HitDirection.GetSafeNormal(), bCriticalHit, bBlocked, bKnockout);
        return;
    }

    PlayHit(HitLocation, HitDirection.GetSafeNormal(), bCriticalHit, bBlocked, bKnockout);
    MulticastPlayHit(HitLocation, HitDirection.GetSafeNormal(), bCriticalHit, bBlocked, bKnockout);
}

void UHitReactionComponent::ServerReceiveHit_Implementation(FVector_NetQuantize HitLocation, FVector_NetQuantizeNormal HitDirection, bool bCriticalHit, bool bBlocked, bool bKnockout)
{
    PlayHit(HitLocation, HitDirection, bCriticalHit, bBlocked, bKnockout);
    MulticastPlayHit(HitLocation, HitDirection, bCriticalHit, bBlocked, bKnockout);
}

void UHitReactionComponent::MulticastPlayHit_Implementation(FVector_NetQuantize HitLocation, FVector_NetQuantizeNormal HitDirection, bool bCriticalHit, bool bBlocked, bool bKnockout)
{
    if (GetOwnerRole() == ROLE_SimulatedProxy)
    {
        PlayHit(HitLocation, HitDirection, bCriticalHit, bBlocked, bKnockout);
    }
}

void UHitReactionComponent::PlayHit(FVector HitLocation, FVector HitDirection, bool bCriticalHit, bool bBlocked, bool bKnockout)
{
    ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
    if (!OwnerChar)
    {
        return;
    }

    float ForwardDot = FVector::DotProduct(OwnerChar->GetActorForwardVector(), HitDirection);
    float RightDot = FVector::DotProduct(OwnerChar->GetActorRightVector(), HitDirection);

    UAnimMontage* Montage = nullptr;
    UNiagaraSystem* FX = nullptr;

    if (FMath::Abs(ForwardDot) > FMath::Abs(RightDot))
    {
        if (ForwardDot > 0.f)
        {
            Montage = FrontHitMontage;
            FX = FrontHitFX;
        }
        else
        {
            Montage = BackHitMontage;
            FX = BackHitFX;
        }
    }
    else
    {
        if (RightDot > 0.f)
        {
            Montage = RightHitMontage;
            FX = RightHitFX;
        }
        else
        {
            Montage = LeftHitMontage;
            FX = LeftHitFX;
        }
    }

    if (Montage)
    {
        OwnerChar->PlayAnimMontage(Montage);
    }

    if (FX)
    {
        UVisualImpactSystem::SpawnDirectionalNiagaraFX(this, FX, HitLocation, HitDirection, bCriticalHit ? 2.f : 1.f, bCriticalHit ? CriticalCameraShake : HitCameraShake);
    }

    if (ImpactDecal)
    {
        UVisualImpactSystem::SpawnImpactDecal(this, ImpactDecal, HitLocation, HitDirection, 1.f, DecalSize, DecalLifeSpan);
    }

    if (bCriticalHit && CriticalSlowMoTime > 0.f)
    {
        StartSlowMotion();
    }

    if (bBlocked && StaminaComponent)
    {
        const float Previous = StaminaComponent->Stamina;
        StaminaComponent->AddStamina(-BlockStaminaCost);
        if (StaminaComponent->Stamina <= 0.f && Previous > 0.f && StaggerMontage)
        {
            OwnerChar->PlayAnimMontage(StaggerMontage);
        }
    }

    if (bKnockout && !bIsKnockedOut)
    {
        bIsKnockedOut = true;
        SetRagdollActive(true);
    }

    if (UWorld* World = GetWorld())
    {
        if (UImpactEventSubsystem* Subsystem = World->GetSubsystem<UImpactEventSubsystem>())
        {
            Subsystem->BroadcastImpact(OwnerChar, HitLocation);
        }
    }
}

void UHitReactionComponent::StartSlowMotion()
{
    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), CriticalSlowMoScale);
    GetWorld()->GetTimerManager().SetTimer(SlowMoTimerHandle, this, &UHitReactionComponent::RestoreTimeDilation, CriticalSlowMoTime, false);
}

void UHitReactionComponent::RestoreTimeDilation()
{
    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
}

void UHitReactionComponent::OnRep_KnockedOut()
{
    if (bIsKnockedOut)
    {
        SetRagdollActive(true);
    }
}

void UHitReactionComponent::OnRep_Ragdoll()
{
    if (bIsRagdoll)
    {
        StartRagdoll();
    }
    else
    {
        StopRagdoll();
    }
}

void UHitReactionComponent::StartRagdoll()
{
    ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
    if (!OwnerChar)
    {
        return;
    }

    if (USkeletalMeshComponent* Mesh = OwnerChar->GetMesh())
    {
        Mesh->SetCollisionProfileName(TEXT("Ragdoll"));
        Mesh->SetAllBodiesSimulatePhysics(true);
        Mesh->SetSimulatePhysics(true);
        Mesh->bBlendPhysics = true;
    }

    if (!bIsRagdoll)
    {
        bIsRagdoll = true;
    }
}

void UHitReactionComponent::StopRagdoll()
{
    ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
    if (!OwnerChar)
    {
        return;
    }

    if (USkeletalMeshComponent* Mesh = OwnerChar->GetMesh())
    {
        Mesh->bBlendPhysics = false;
        Mesh->SetSimulatePhysics(false);
        Mesh->SetAllBodiesSimulatePhysics(false);
        Mesh->SetCollisionProfileName(TEXT("CharacterMesh"));
    }
}

void UHitReactionComponent::SetRagdollActive(bool bActive)
{
    if (GetOwnerRole() < ROLE_Authority)
    {
        ServerSetRagdollActive(bActive);
        return;
    }

    bIsRagdoll = bActive;
    OnRep_Ragdoll();
}

void UHitReactionComponent::ServerSetRagdollActive_Implementation(bool bActive)
{
    SetRagdollActive(bActive);
}

void UHitReactionComponent::Revive()
{
    if (GetOwnerRole() < ROLE_Authority)
    {
        ServerRevive();
        return;
    }

    bIsKnockedOut = false;
    SetRagdollActive(false);
    MulticastOnRevive();
}

void UHitReactionComponent::ServerRevive_Implementation()
{
    Revive();
}

void UHitReactionComponent::MulticastOnRevive_Implementation()
{
    OnRevive();
}

void UHitReactionComponent::AddStamina(float Amount)
{
    if (StaminaComponent)
    {
        StaminaComponent->AddStamina(Amount);
    }
}

void UHitReactionComponent::SetMaxStamina(float NewMax)
{
    if (StaminaComponent)
    {
        StaminaComponent->SetMaxStamina(NewMax);
    }
}

