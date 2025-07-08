#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Components/ActorComponent.h"
#include "MissionManagerComponent.generated.h"

USTRUCT(BlueprintType)
struct FMissionData : public FTableRowBase
{
    GENERATED_BODY();

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName MissionId;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 StageCount = 1;
};

USTRUCT(BlueprintType)
struct FMissionProgress
{
    GENERATED_BODY();

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName MissionId;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 CurrentStage = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bCompleted = false;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMissionUpdated, const FMissionProgress&, Progress);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ALSREPLICATED_API UMissionManagerComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UMissionManagerComponent();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void BeginPlay() override;

    /** Advance the specified mission to the next stage */
    UFUNCTION(BlueprintCallable, Category="Mission")
    void AdvanceMission(FName MissionId);

    /** Retrieve mission data from table or loaded JSON */
    UFUNCTION(BlueprintCallable, Category="Mission")
    const FMissionData* GetMissionData(FName MissionId) const;

    /** Mission data table asset */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Mission")
    UDataTable* MissionTable = nullptr;

    /** Broadcast whenever mission progress changes */
    UPROPERTY(BlueprintAssignable)
    FMissionUpdated OnMissionUpdated;

protected:
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerAdvanceMission(FName MissionId);

    UFUNCTION()
    void OnRep_MissionProgress();

    void LoadMissionsFromJson();

    /** Replicated mission progress array */
    UPROPERTY(ReplicatedUsing=OnRep_MissionProgress)
    TArray<FMissionProgress> MissionProgress;

    /** Data parsed from JSON files */
    UPROPERTY()
    TArray<FMissionData> JsonMissions;
};

