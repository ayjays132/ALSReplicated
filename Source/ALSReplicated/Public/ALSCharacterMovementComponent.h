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
	UPROPERTY(Replicated)
	float NewMaxWalkSpeed = MaxWalkSpeed;
	
	UPROPERTY(Replicated)
        float NewMaxWalkSpeedCrouched = MaxWalkSpeedCrouched;

	UPROPERTY(Replicated)
	float NewMaxAcceleration = MaxAcceleration;

	UPROPERTY(Replicated)
	float NewBrakingDecelerationWalking = BrakingDecelerationWalking;
	
	UPROPERTY(Replicated)
    float NewGroundFriction = GroundFriction;

	UPROPERTY(Replicated)
        float NewBrakingFrictionFactor = BrakingFrictionFactor;

	UFUNCTION(Server, Reliable)
	void Server_MaxWalkSpeed(float Max_WalkSpeed);

	UFUNCTION(Server, Reliable)
        void Server_MaxWalkSpeedCrouched(float Max_WalkSpeedCrouched);

	UFUNCTION(Server, Reliable)
	void Server_MaxAcceleration(float Max_Acceleration);

	UFUNCTION(Server, Reliable)
	void Server_BrakingDecelerationWalking(float Braking_DecelerationWalking);

	UFUNCTION(Server, Reliable)
	void Server_GroundFriction(float Ground_Friction);

	UFUNCTION(Server, Reliable)
        void Server_BrakingFrictionFactor(float Braking_FrictionFactor);
	
};

