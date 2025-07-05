// Fill out your copyright notice in the Description page of Project Settings.


#include "ALSBaseCharacter.h"
#include "LockOnComponent.h"
#include "CombatComponent.h"
#include "EnvironmentInteractionComponent.h"
#include "HitReactionComponent.h"

// Sets default values
AALSBaseCharacter::AALSBaseCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UALSCharacterMovementComponent>(CharacterMovementComponentName))
{
        // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
        PrimaryActorTick.bCanEverTick = true;

       LockOn = CreateDefaultSubobject<ULockOnComponent>(TEXT("LockOnComponent"));
        CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
        InteractionComponent = CreateDefaultSubobject<UEnvironmentInteractionComponent>(TEXT("EnvironmentInteractionComponent"));
        HitReactionComponent = CreateDefaultSubobject<UHitReactionComponent>(TEXT("HitReactionComponent"));
}

// Called when the game starts or when spawned
void AALSBaseCharacter::BeginPlay()
{
        Super::BeginPlay();

       ControlRotation = GetControlRotation();

	ALSCharacterMovement = Cast<UALSCharacterMovementComponent>(GetCharacterMovement());

	if (GetLocalRole() == ROLE_SimulatedProxy)
	{
		GetMesh()->GetAnimInstance()->SetRootMotionMode(ERootMotionMode::IgnoreRootMotion);
	}

	SetReplicateMovement(true);

	if (HasAuthority())
	{
		SetReplicates(true);
	}
}

// Called every frame
void AALSBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetLocalRole() != ROLE_SimulatedProxy)
	{
		ControlRotation = FMath::RInterpTo(ControlRotation, GetControlRotation(), DeltaTime, 30.0f);
	}
}

// Called to bind functionality to input
void AALSBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
       Super::SetupPlayerInputComponent(PlayerInputComponent);

       if (PlayerInputComponent)
       {
               PlayerInputComponent->BindAction(TEXT("LockOnAction"), IE_Pressed, this, &AALSBaseCharacter::ToggleLockOn);
               PlayerInputComponent->BindAction(TEXT("UseAction"), IE_Pressed, this, &AALSBaseCharacter::UseAction);
               if (CombatComponent)
               {
                       PlayerInputComponent->BindAction(TEXT("LightAttack"), IE_Pressed, CombatComponent, &UCombatComponent::LightAttack);
                       PlayerInputComponent->BindAction(TEXT("HeavyAttack"), IE_Pressed, CombatComponent, &UCombatComponent::HeavyAttack);
               }
       }
}

void AALSBaseCharacter::ToggleLockOn()
{
       if (LockOn)
       {
               LockOn->ToggleLockOn();
       }
}

void AALSBaseCharacter::EquipWeapon(AActor* Weapon, FName SocketName)
{
       if (CombatComponent)
       {
               CombatComponent->EquipWeapon(Weapon, SocketName);
       }
}

void AALSBaseCharacter::UnequipWeapon()
{
       if (CombatComponent)
       {
               CombatComponent->UnequipWeapon();
       }
}

void AALSBaseCharacter::UseAction()
{
       if (InteractionComponent)
       {
               InteractionComponent->UseAction();
       }
}

void AALSBaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AALSBaseCharacter, ControlRotation, COND_SkipOwner);
}

