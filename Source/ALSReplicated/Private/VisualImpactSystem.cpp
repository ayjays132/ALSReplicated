#include "VisualImpactSystem.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

void UVisualImpactSystem::SpawnDirectionalNiagaraFX(UObject* WorldContextObject, UNiagaraSystem* FX, const FVector& Location, const FVector& Direction, float Force, TSubclassOf<UCameraShakeBase> CameraShake, float ShakeThreshold)
{
    if (!WorldContextObject || !FX)
    {
        return;
    }

    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject);
    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("SpawnDirectionalNiagaraFX: Invalid world context object"));
        return;
    }

    UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, FX, Location, Direction.Rotation());

    if (CameraShake && Force >= ShakeThreshold)
    {
        if (APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0))
        {
            PC->ClientStartCameraShake(CameraShake);
        }
    }
}

void UVisualImpactSystem::SpawnImpactDecal(UObject* WorldContextObject, UMaterialInterface* Decal, const FVector& Location, const FVector& Direction, float Force, FVector Size, float LifeSpan)
{
    if (!WorldContextObject || !Decal)
    {
        return;
    }

    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject);
    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("SpawnImpactDecal: Invalid world context object"));
        return;
    }

    UGameplayStatics::SpawnDecalAtLocation(World, Decal, Size, Location, Direction.Rotation(), LifeSpan);
}

