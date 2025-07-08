// Fill out your copyright notice in the Description page of Project Settings.


#include "ALSCharacterMovementComponent.h"

void UALSCharacterMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(UALSCharacterMovementComponent, NewMaxWalkSpeed, COND_SkipOwner);
        DOREPLIFETIME_CONDITION(UALSCharacterMovementComponent, NewMaxWalkSpeedCrouched, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(UALSCharacterMovementComponent, NewMaxAcceleration, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(UALSCharacterMovementComponent, NewBrakingDecelerationWalking, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(UALSCharacterMovementComponent, NewGroundFriction, COND_SkipOwner);
        DOREPLIFETIME_CONDITION(UALSCharacterMovementComponent, NewBrakingFrictionFactor, COND_SkipOwner);
}

void UALSCharacterMovementComponent::SetMaxWalkSpeed(float Max_WalkSpeed)
{
    if (GetOwnerRole() < ROLE_Authority)
    {
        Server_MaxWalkSpeed(Max_WalkSpeed);
        return;
    }
    NewMaxWalkSpeed = Max_WalkSpeed;
    MaxWalkSpeed = NewMaxWalkSpeed;
}

void UALSCharacterMovementComponent::SetMaxWalkSpeedCrouched(float Max_WalkSpeedCrouched)
{
    if (GetOwnerRole() < ROLE_Authority)
    {
        Server_MaxWalkSpeedCrouched(Max_WalkSpeedCrouched);
        return;
    }
    NewMaxWalkSpeedCrouched = Max_WalkSpeedCrouched;
    MaxWalkSpeedCrouched = NewMaxWalkSpeedCrouched;
}

void UALSCharacterMovementComponent::SetMaxAcceleration(float Max_Acceleration)
{
    if (GetOwnerRole() < ROLE_Authority)
    {
        Server_MaxAcceleration(Max_Acceleration);
        return;
    }
    NewMaxAcceleration = Max_Acceleration;
    MaxAcceleration = NewMaxAcceleration;
}

void UALSCharacterMovementComponent::SetBrakingDecelerationWalking(float Braking_DecelerationWalking)
{
    if (GetOwnerRole() < ROLE_Authority)
    {
        Server_BrakingDecelerationWalking(Braking_DecelerationWalking);
        return;
    }
    NewBrakingDecelerationWalking = Braking_DecelerationWalking;
    BrakingDecelerationWalking = NewBrakingDecelerationWalking;
}

void UALSCharacterMovementComponent::SetGroundFriction(float Ground_Friction)
{
    if (GetOwnerRole() < ROLE_Authority)
    {
        Server_GroundFriction(Ground_Friction);
        return;
    }
    NewGroundFriction = Ground_Friction;
    GroundFriction = NewGroundFriction;
}

void UALSCharacterMovementComponent::SetBrakingFrictionFactor(float Braking_FrictionFactor)
{
    if (GetOwnerRole() < ROLE_Authority)
    {
        Server_BrakingFrictionFactor(Braking_FrictionFactor);
        return;
    }
    NewBrakingFrictionFactor = Braking_FrictionFactor;
    BrakingFrictionFactor = NewBrakingFrictionFactor;
}

void UALSCharacterMovementComponent::Server_MaxWalkSpeed_Implementation(float Max_WalkSpeed)
{
    SetMaxWalkSpeed(Max_WalkSpeed);
}

void UALSCharacterMovementComponent::Server_MaxWalkSpeedCrouched_Implementation(float Max_WalkSpeedCrouched)
{
    SetMaxWalkSpeedCrouched(Max_WalkSpeedCrouched);
}

void UALSCharacterMovementComponent::Server_MaxAcceleration_Implementation(float Max_Acceleration)
{
    SetMaxAcceleration(Max_Acceleration);
}

void UALSCharacterMovementComponent::Server_BrakingDecelerationWalking_Implementation(float Braking_DecelerationWalking)
{
    SetBrakingDecelerationWalking(Braking_DecelerationWalking);
}

void UALSCharacterMovementComponent::Server_GroundFriction_Implementation(float Ground_Friction)
{
    SetGroundFriction(Ground_Friction);
}

void UALSCharacterMovementComponent::Server_BrakingFrictionFactor_Implementation(float Braking_FrictionFactor)
{
    SetBrakingFrictionFactor(Braking_FrictionFactor);
}

bool UALSCharacterMovementComponent::Server_MaxWalkSpeed_Validate(float Max_WalkSpeed)
{
    return GetOwner() && GetOwner()->HasAuthority() && FMath::IsFinite(Max_WalkSpeed) && Max_WalkSpeed >= 0.f;
}

bool UALSCharacterMovementComponent::Server_MaxWalkSpeedCrouched_Validate(float Max_WalkSpeedCrouched)
{
    return GetOwner() && GetOwner()->HasAuthority() && FMath::IsFinite(Max_WalkSpeedCrouched) && Max_WalkSpeedCrouched >= 0.f;
}

bool UALSCharacterMovementComponent::Server_MaxAcceleration_Validate(float Max_Acceleration)
{
    return GetOwner() && GetOwner()->HasAuthority() && FMath::IsFinite(Max_Acceleration) && Max_Acceleration >= 0.f;
}

bool UALSCharacterMovementComponent::Server_BrakingDecelerationWalking_Validate(float Braking_DecelerationWalking)
{
    return GetOwner() && GetOwner()->HasAuthority() && FMath::IsFinite(Braking_DecelerationWalking) && Braking_DecelerationWalking >= 0.f;
}

bool UALSCharacterMovementComponent::Server_GroundFriction_Validate(float Ground_Friction)
{
    return GetOwner() && GetOwner()->HasAuthority() && FMath::IsFinite(Ground_Friction) && Ground_Friction >= 0.f;
}

bool UALSCharacterMovementComponent::Server_BrakingFrictionFactor_Validate(float Braking_FrictionFactor)
{
    return GetOwner() && GetOwner()->HasAuthority() && FMath::IsFinite(Braking_FrictionFactor) && Braking_FrictionFactor >= 0.f;
}

void UALSCharacterMovementComponent::OnRep_NewMaxWalkSpeed()
{
    MaxWalkSpeed = NewMaxWalkSpeed;
}

void UALSCharacterMovementComponent::OnRep_NewMaxWalkSpeedCrouched()
{
    MaxWalkSpeedCrouched = NewMaxWalkSpeedCrouched;
}

void UALSCharacterMovementComponent::OnRep_NewMaxAcceleration()
{
    MaxAcceleration = NewMaxAcceleration;
}

void UALSCharacterMovementComponent::OnRep_NewBrakingDecelerationWalking()
{
    BrakingDecelerationWalking = NewBrakingDecelerationWalking;
}

void UALSCharacterMovementComponent::OnRep_NewGroundFriction()
{
    GroundFriction = NewGroundFriction;
}

void UALSCharacterMovementComponent::OnRep_NewBrakingFrictionFactor()
{
    BrakingFrictionFactor = NewBrakingFrictionFactor;
}
