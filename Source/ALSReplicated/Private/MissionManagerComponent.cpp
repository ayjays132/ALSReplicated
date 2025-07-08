#include "MissionManagerComponent.h"
#include "Net/UnrealNetwork.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "JsonObjectConverter.h"

UMissionManagerComponent::UMissionManagerComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

void UMissionManagerComponent::BeginPlay()
{
    Super::BeginPlay();
    LoadMissionsFromJson();
}

void UMissionManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UMissionManagerComponent, MissionProgress);
}

const FMissionData* UMissionManagerComponent::GetMissionData(FName MissionId) const
{
    if (MissionTable)
    {
        return MissionTable->FindRow<FMissionData>(MissionId, TEXT("MissionLookup"));
    }

    for (const FMissionData& Data : JsonMissions)
    {
        if (Data.MissionId == MissionId)
        {
            return &Data;
        }
    }
    return nullptr;
}

void UMissionManagerComponent::AdvanceMission(FName MissionId)
{
    if (GetOwnerRole() < ROLE_Authority)
    {
        ServerAdvanceMission(MissionId);
        return;
    }

    for (FMissionProgress& Prog : MissionProgress)
    {
        if (Prog.MissionId == MissionId)
        {
            Prog.CurrentStage++;
            const FMissionData* Data = GetMissionData(MissionId);
            if (Data && Prog.CurrentStage >= Data->StageCount)
            {
                Prog.bCompleted = true;
            }
            OnRep_MissionProgress();
            return;
        }
    }

    FMissionProgress NewProgress;
    NewProgress.MissionId = MissionId;
    NewProgress.CurrentStage = 1;
    const FMissionData* DataPtr = GetMissionData(MissionId);
    if (DataPtr && NewProgress.CurrentStage >= DataPtr->StageCount)
    {
        NewProgress.bCompleted = true;
    }
    MissionProgress.Add(NewProgress);
    OnRep_MissionProgress();
}

bool UMissionManagerComponent::ServerAdvanceMission_Validate(FName MissionId)
{
    return true;
}

void UMissionManagerComponent::ServerAdvanceMission_Implementation(FName MissionId)
{
    AdvanceMission(MissionId);
}

void UMissionManagerComponent::OnRep_MissionProgress()
{
    for (const FMissionProgress& Prog : MissionProgress)
    {
        OnMissionUpdated.Broadcast(Prog);
    }
}

void UMissionManagerComponent::LoadMissionsFromJson()
{
    FString QuestDir = FPaths::ProjectContentDir() / TEXT("Quests/");
    TArray<FString> Files;
    IFileManager::Get().FindFiles(Files, *QuestDir, TEXT("json"));

    for (const FString& File : Files)
    {
        FString Json;
        FString Path = QuestDir / File;
        if (FFileHelper::LoadFileToString(Json, *Path))
        {
            FMissionData Data;
            if (FJsonObjectConverter::JsonObjectStringToUStruct(Json, &Data, 0, 0))
            {
                JsonMissions.Add(Data);
            }
        }
    }
}

