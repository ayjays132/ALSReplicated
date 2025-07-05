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
	NewMaxWalkSpeed = Max_WalkSpeed;
	Server_MaxWalkSpeed_Implementation(NewMaxWalkSpeed);
	MaxWalkSpeed = NewMaxWalkSpeed;
}

void UALSCharacterMovementComponent::SetMaxWalkSpeedCrouched(float Max_WalkSpeedCrouched)
{
        NewMaxWalkSpeedCrouched = Max_WalkSpeedCrouched;
        Server_MaxWalkSpeedCrouched_Implementation(NewMaxWalkSpeedCrouched);
        MaxWalkSpeedCrouched = NewMaxWalkSpeedCrouched;
}

void UALSCharacterMovementComponent::SetMaxAcceleration(float Max_Acceleration)
{
	NewMaxAcceleration = Max_Acceleration;
	Server_MaxAcceleration_Implementation(Max_Acceleration);
	MaxAcceleration = NewMaxAcceleration;
}

void UALSCharacterMovementComponent::SetBrakingDecelerationWalking(float Braking_DecelerationWalking)
{
	NewBrakingDecelerationWalking = Braking_DecelerationWalking;
	Server_BrakingDecelerationWalking_Implementation(NewBrakingDecelerationWalking);
	BrakingDecelerationWalking = NewBrakingDecelerationWalking;
}

void UALSCharacterMovementComponent::SetGroundFriction(float Ground_Friction)
{
	NewGroundFriction = Ground_Friction;
	Server_GroundFriction_Implementation(NewGroundFriction);
	GroundFriction = NewGroundFriction;
}

void UALSCharacterMovementComponent::SetBrakingFrictionFactor(float Braking_FrictionFactor)
{
        NewBrakingFrictionFactor = Braking_FrictionFactor;
        Server_BrakingFrictionFactor_Implementation(NewBrakingFrictionFactor);
        BrakingFrictionFactor = NewBrakingFrictionFactor;
}

void UALSCharacterMovementComponent::Server_MaxWalkSpeed_Implementation(float Max_WalkSpeed)
{
	NewMaxWalkSpeed = Max_WalkSpeed;
}

void UALSCharacterMovementComponent::Server_MaxWalkSpeedCrouched_Implementation(float Max_WalkSpeedCrouched)
{
        MaxWalkSpeedCrouched = Max_WalkSpeedCrouched;
}

void UALSCharacterMovementComponent::Server_MaxAcceleration_Implementation(float Max_Acceleration)
{
	NewMaxAcceleration = Max_Acceleration;
}

void UALSCharacterMovementComponent::Server_BrakingDecelerationWalking_Implementation(float Braking_DecelerationWalking)
{
	NewBrakingDecelerationWalking = Braking_DecelerationWalking;
}

void UALSCharacterMovementComponent::Server_GroundFriction_Implementation(float Ground_Friction)
{
	NewGroundFriction = Ground_Friction;
}

void UALSCharacterMovementComponent::Server_BrakingFrictionFactor_Implementation(float Braking_FrictionFactor)
{
        NewBrakingFrictionFactor = Braking_FrictionFactor;
}
