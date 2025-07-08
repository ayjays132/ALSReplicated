// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "ALSCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class ALSREPLICATED_API UALSCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category="Movement Component")
	void SetMaxWalkSpeed(float Max_WalkSpeed);

        UFUNCTION(BlueprintCallable, Category="Movement Component")
        void SetMaxWalkSpeedCrouched(float Max_WalkSpeedCrouched);

	UFUNCTION(BlueprintCallable, Category="Movement Component")
	void SetMaxAcceleration(float Max_Acceleration);
	
	UFUNCTION(BlueprintCallable, Category="Movement Component")
    void SetBrakingDecelerationWalking(float Braking_DecelerationWalking);
    	
	UFUNCTION(BlueprintCallable, Category="Movement Component")
	void SetGroundFriction(float Ground_Friction);

        UFUNCTION(BlueprintCallable, Category="Movement Component")
        void SetBrakingFrictionFactor(float Braking_FrictionFactor);
	
private:
       UFUNCTION()
       void OnRep_NewMaxWalkSpeed();

       UFUNCTION()
       void OnRep_NewMaxWalkSpeedCrouched();

       UFUNCTION()
       void OnRep_NewMaxAcceleration();

       UFUNCTION()
       void OnRep_NewBrakingDecelerationWalking();

       UFUNCTION()
       void OnRep_NewGroundFriction();

       UFUNCTION()
       void OnRep_NewBrakingFrictionFactor();

       UPROPERTY(ReplicatedUsing=OnRep_NewMaxWalkSpeed)
       float NewMaxWalkSpeed = MaxWalkSpeed;

       UPROPERTY(ReplicatedUsing=OnRep_NewMaxWalkSpeedCrouched)
       float NewMaxWalkSpeedCrouched = MaxWalkSpeedCrouched;

       UPROPERTY(ReplicatedUsing=OnRep_NewMaxAcceleration)
       float NewMaxAcceleration = MaxAcceleration;

       UPROPERTY(ReplicatedUsing=OnRep_NewBrakingDecelerationWalking)
       float NewBrakingDecelerationWalking = BrakingDecelerationWalking;

       UPROPERTY(ReplicatedUsing=OnRep_NewGroundFriction)
   float NewGroundFriction = GroundFriction;

       UPROPERTY(ReplicatedUsing=OnRep_NewBrakingFrictionFactor)
       float NewBrakingFrictionFactor = BrakingFrictionFactor;

         UFUNCTION(Server, Reliable, WithValidation)
         void Server_MaxWalkSpeed(float Max_WalkSpeed);

         UFUNCTION(Server, Reliable, WithValidation)
         void Server_MaxWalkSpeedCrouched(float Max_WalkSpeedCrouched);

         UFUNCTION(Server, Reliable, WithValidation)
         void Server_MaxAcceleration(float Max_Acceleration);

         UFUNCTION(Server, Reliable, WithValidation)
         void Server_BrakingDecelerationWalking(float Braking_DecelerationWalking);

         UFUNCTION(Server, Reliable, WithValidation)
         void Server_GroundFriction(float Ground_Friction);

         UFUNCTION(Server, Reliable, WithValidation)
         void Server_BrakingFrictionFactor(float Braking_FrictionFactor);
	
};

