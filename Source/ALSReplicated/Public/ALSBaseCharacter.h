// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ALSCharacterMovementComponent.h"
#include "LockOnComponent.h"
#include "CombatComponent.h"
#include "EnvironmentInteractionComponent.h"
#include "HitReactionComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "ALSBaseCharacter.generated.h"

UCLASS()
class ALSREPLICATED_API AALSBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AALSBaseCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadWrite, Replicated, Category="ALS || Rotation")
	FRotator ControlRotation = GetControlRotation();

        UPROPERTY(BlueprintReadWrite, Category="ALS || Component")
        UALSCharacterMovementComponent* ALSCharacterMovement;

        UPROPERTY(BlueprintReadWrite, Category="ALS || Component")
        ULockOnComponent* LockOnComponent;

        UPROPERTY(BlueprintReadWrite, Category="ALS || Component")
        UCombatComponent* CombatComponent;

        UPROPERTY(BlueprintReadWrite, Category="ALS || Component")
        UEnvironmentInteractionComponent* InteractionComponent;

        UPROPERTY(BlueprintReadWrite, Category="ALS || Component")
        UHitReactionComponent* HitReactionComponent;

};

