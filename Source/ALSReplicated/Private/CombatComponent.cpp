#include "CombatComponent.h"
#include "GameFramework/Character.h"

UCombatComponent::UCombatComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

void UCombatComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UCombatComponent, bIsAttacking);
    DOREPLIFETIME(UCombatComponent, ComboIndex);
    DOREPLIFETIME(UCombatComponent, AttackCooldown);
}

void UCombatComponent::LightAttack()
{
    if (bIsAttacking || AttackCooldown > 0.f)
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
    ServerStartAttack(true);
}

void UCombatComponent::ServerStartAttack_Implementation(bool bHeavy)
{
    DoAttack(bHeavy);
}

void UCombatComponent::DoAttack(bool bHeavy)
{
    bIsAttacking = true;
    ComboIndex++;

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
    // This function can be extended to react to attack state changes
}

void UCombatComponent::EquipWeapon(AActor* Weapon, FName SocketName)
{
    if (!Weapon)
    {
        return;
    }

    if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
    {
        Weapon->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
        EquippedWeapon = Weapon;
        EquippedSocket = SocketName;
    }
}

void UCombatComponent::UnequipWeapon()
{
    if (EquippedWeapon)
    {
        EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        EquippedWeapon = nullptr;
    }
}

void UCombatComponent::SpawnHitbox()
{
    // Placeholder: spawn hitbox for attack, triggered by AnimNotify
}

