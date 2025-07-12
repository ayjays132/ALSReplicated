#include "CombatComponent.h"
#include "GameFramework/Character.h"
#include "ALSCharacterMovementComponent.h"
#include "StaminaComponent.h"
#include "WeaponComponent.h"

UCombatComponent::UCombatComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

void UCombatComponent::BeginPlay()
{
    Super::BeginPlay();

    if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
    {
        CachedMovement = Cast<UALSCharacterMovementComponent>(OwnerCharacter->GetCharacterMovement());
        if (CachedMovement)
        {
            PreviousWalkSpeed = CachedMovement->MaxWalkSpeed;
        }
        StaminaComponent = OwnerCharacter->FindComponentByClass<UStaminaComponent>();
    }
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UCombatComponent, bIsAttacking);
    DOREPLIFETIME(UCombatComponent, ComboIndex);
    DOREPLIFETIME(UCombatComponent, AttackCooldown);
    DOREPLIFETIME(UCombatComponent, bUseDashRootMotion);
}

void UCombatComponent::LightAttack()
{
    if (bIsAttacking || AttackCooldown > 0.f)
    {
        return;
    }
    if (!StaminaComponent || StaminaComponent->Stamina < LightAttackStaminaCost)
    {
        return;
    }
    ServerStartAttack(false);
}

void UCombatComponent::HeavyAttack()
{
    if (bIsAttacking || AttackCooldown > 0.f)
    {
        return;
    }
    if (!StaminaComponent || StaminaComponent->Stamina < HeavyAttackStaminaCost)
    {
        return;
    }
    if (CachedMovement && CachedMovement->Velocity.SizeSquared() > 0.f)
    {
        PerformDash();
    }
    ServerStartAttack(true);
}

void UCombatComponent::ServerStartAttack_Implementation(bool bHeavy)
{
    DoAttack(bHeavy);
}

bool UCombatComponent::ServerStartAttack_Validate(bool bHeavy)
{
    return GetOwner() && GetOwner()->HasAuthority();
}

void UCombatComponent::DoAttack(bool bHeavy)
{
    bIsAttacking = true;
    ComboIndex++;

    if (StaminaComponent)
    {
        StaminaComponent->AddStamina(-(bHeavy ? HeavyAttackStaminaCost : LightAttackStaminaCost));
    }

    if (CachedMovement)
    {
        PreviousWalkSpeed = CachedMovement->MaxWalkSpeed;
        CachedMovement->SetMaxWalkSpeed(AttackWalkSpeed);
    }

    OnRep_AttackState();

    GetWorld()->GetTimerManager().ClearTimer(ComboTimerHandle);
    GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &UCombatComponent::ResetCombo, ComboResetTime, false);
    GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &UCombatComponent::FinishAttack, AttackDuration, false);

    UAnimMontage* MontageToPlay = bHeavy ? HeavyAttackMontage : LightAttackMontage;
    if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
    {
        if (MontageToPlay)
        {
            OwnerCharacter->PlayAnimMontage(MontageToPlay);
        }
    }
}

void UCombatComponent::FinishAttack()
{
    bIsAttacking = false;
    AttackCooldown = AttackDuration;
    OnRep_AttackState();

    if (CachedMovement)
    {
        CachedMovement->SetMaxWalkSpeed(PreviousWalkSpeed);
    }

    GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, [this]()
    {
        AttackCooldown = 0.f;
    }, AttackCooldown, false);
}

void UCombatComponent::ResetCombo()
{
    ComboIndex = 0;
}

void UCombatComponent::OnRep_AttackState()
{
    OnAttackStateChanged(bIsAttacking);
}


void UCombatComponent::EquipWeapon(UWeaponComponent* Weapon, FName SocketName)
{
    if (!Weapon)
    {
        return;
    }

    if (GetOwnerRole() < ROLE_Authority)
    {
        ServerEquipWeapon(Weapon, SocketName);
        return;
    }

    if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
    {
        if (AActor* WeaponActor = Weapon->GetOwner())
        {
            WeaponActor->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
        }
        EquippedWeapon = Weapon;
        EquippedSocket = SocketName;
    }
}

void UCombatComponent::UnequipWeapon()
{
    if (GetOwnerRole() < ROLE_Authority)
    {
        ServerUnequipWeapon();
        return;
    }

    if (EquippedWeapon)
    {
        if (AActor* WeaponActor = EquippedWeapon->GetOwner())
        {
            WeaponActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        }
        EquippedWeapon = nullptr;
    }
}

void UCombatComponent::ServerEquipWeapon_Implementation(UWeaponComponent* Weapon, FName SocketName)
{
    EquipWeapon(Weapon, SocketName);
}

bool UCombatComponent::ServerEquipWeapon_Validate(UWeaponComponent* Weapon, FName SocketName)
{
    return GetOwner() && GetOwner()->HasAuthority() && Weapon != nullptr;
}

void UCombatComponent::ServerUnequipWeapon_Implementation()
{
    UnequipWeapon();
}

bool UCombatComponent::ServerUnequipWeapon_Validate()
{
    return GetOwner() && GetOwner()->HasAuthority();
}

void UCombatComponent::SpawnHitbox()
{
    if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
    {
        const FVector Start = OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorForwardVector() * 50.f;
        const FVector End = Start + OwnerCharacter->GetActorForwardVector() * 100.f;
        FCollisionShape Sphere = FCollisionShape::MakeSphere(50.f);
        TArray<FHitResult> Hits;
        if (GetWorld()->SweepMultiByChannel(Hits, Start, End, FQuat::Identity, ECC_Pawn, Sphere))
        {
            for (const FHitResult& Hit : Hits)
            {
                if (Hit.GetActor() && Hit.GetActor() != OwnerCharacter)
                {
                    OnHitDetected(Hit);
                }
            }
        }
    }
}

void UCombatComponent::AddStamina(float Amount)
{
    if (StaminaComponent)
    {
        StaminaComponent->AddStamina(Amount);
    }
}

void UCombatComponent::SetMaxStamina(float NewMax)
{
    if (StaminaComponent)
    {
        StaminaComponent->SetMaxStamina(NewMax);
    }
}

void UCombatComponent::PerformDash()
{
    if (!bUseDashRootMotion || !DashMontage || !CachedMovement)
    {
        return;
    }

    if (GetOwnerRole() < ROLE_Authority)
    {
        ServerPerformDash();
        return;
    }

    MulticastPerformDash();
}

void UCombatComponent::ServerPerformDash_Implementation()
{
    PerformDash();
}

void UCombatComponent::MulticastPerformDash_Implementation()
{
    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (!OwnerCharacter || !DashMontage || !CachedMovement)
    {
        return;
    }

    UAnimInstance* AnimInst = OwnerCharacter->GetMesh() ? OwnerCharacter->GetMesh()->GetAnimInstance() : nullptr;
    if (!AnimInst)
    {
        return;
    }

    const EMovementMode PrevMode = CachedMovement->MovementMode;
    const ERootMotionMode::Type PrevRoot = AnimInst->GetRootMotionMode();

    AnimInst->SetRootMotionMode(ERootMotionMode::RootMotionFromMontagesOnly);
    const float Duration = OwnerCharacter->PlayAnimMontage(DashMontage);
    CachedMovement->SetMovementMode(MOVE_Flying);

    GetWorld()->GetTimerManager().SetTimer(DashTimerHandle, [this, AnimInst, PrevRoot, PrevMode]()
    {
        if (AnimInst)
        {
            AnimInst->SetRootMotionMode(PrevRoot);
        }
        if (CachedMovement)
        {
            CachedMovement->SetMovementMode(PrevMode);
        }
    }, Duration, false);
}

