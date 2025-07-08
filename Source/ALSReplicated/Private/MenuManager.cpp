#include "MenuManager.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameUserSettings.h"
#include "AccessibilitySettings.h"

void UMenuManager::ShowMainMenu()
{
    ShowWidget(MainMenuClass);
}

void UMenuManager::ShowOptionsMenu()
{
    ShowWidget(OptionsMenuClass);
}

void UMenuManager::ShowAccessibilityMenu()
{
    ShowWidget(AccessibilityOptionsClass);
}

void UMenuManager::ShowKeybindMenu()
{
    ShowWidget(KeybindMenuClass);
}

void UMenuManager::SaveSettings()
{
    if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
    {
        Settings->ApplySettings(false);
        Settings->SaveSettings();
    }

    if (UAccessibilitySettings* AccessSettings = UAccessibilitySettings::Get())
    {
        AccessSettings->Save();
    }
}

void UMenuManager::Deinitialize()
{
    if (CurrentWidget)
    {
        CurrentWidget->RemoveFromParent();
        CurrentWidget = nullptr;
    }
    Super::Deinitialize();
}

void UMenuManager::ShowWidget(TSubclassOf<UUserWidget> WidgetClass)
{
    if (!WidgetClass)
    {
        return;
    }
    if (CurrentWidget)
    {
        CurrentWidget->RemoveFromParent();
        CurrentWidget = nullptr;
    }
    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0))
    {
        CurrentWidget = CreateWidget<UUserWidget>(PC, WidgetClass);
        if (CurrentWidget)
        {
            CurrentWidget->AddToViewport();
            FInputModeUIOnly InputMode;
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            InputMode.SetWidgetToFocus(CurrentWidget->TakeWidget());
            PC->SetInputMode(InputMode);
            PC->bShowMouseCursor = true;
        }
    }
}
