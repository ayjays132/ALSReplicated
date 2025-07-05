#include "CombatComponent.h"
#include "GameFramework/Character.h"
#include "ALSCharacterMovementComponent.h"

UCombatComponent::UCombatComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

void UCombatComponent::BeginPlay()
{
    Super::BeginPlay();

    Stamina = MaxStamina;

    if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
    {
        CachedMovement = Cast<UALSCharacterMovementComponent>(OwnerCharacter->GetCharacterMovement());
        if (CachedMovement)
        {
            PreviousWalkSpeed = CachedMovement->MaxWalkSpeed;
        }
    }
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UCombatComponent, bIsAttacking);
    DOREPLIFETIME(UCombatComponent, ComboIndex);
    DOREPLIFETIME(UCombatComponent, AttackCooldown);
    DOREPLIFETIME(UCombatComponent, Stamina);
}

void UCombatComponent::LightAttack()
{
    if (bIsAttacking || AttackCooldown > 0.f)
    {
        return;
    }
    if (Stamina < LightAttackStaminaCost)
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
    if (Stamina < HeavyAttackStaminaCost)
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

    Stamina = FMath::Clamp(Stamina - (bHeavy ? HeavyAttackStaminaCost : LightAttackStaminaCost), 0.f, MaxStamina);

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

void UCombatComponent::EquipWeapon(AActor* Weapon, FName SocketName)
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
        Weapon->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
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
        EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        EquippedWeapon = nullptr;
    }
}

void UCombatComponent::ServerEquipWeapon_Implementation(AActor* Weapon, FName SocketName)
{
    EquipWeapon(Weapon, SocketName);
}

void UCombatComponent::ServerUnequipWeapon_Implementation()
{
    UnequipWeapon();
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
    Stamina = FMath::Clamp(Stamina + Amount, 0.f, MaxStamina);
}

void UCombatComponent::SetMaxStamina(float NewMax)
{
    MaxStamina = NewMax;
    Stamina = FMath::Clamp(Stamina, 0.f, MaxStamina);
}

