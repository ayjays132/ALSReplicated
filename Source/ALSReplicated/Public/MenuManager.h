#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MenuManager.generated.h"

class UUserWidget;

/** Simple subsystem used to toggle UI menus and persist user settings. */
UCLASS()
class ALSREPLICATED_API UMenuManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:
    /** Show the main menu widget */
    UFUNCTION(BlueprintCallable, Category="Menu")
    void ShowMainMenu();

    /** Show the graphics and audio options menu */
    UFUNCTION(BlueprintCallable, Category="Menu")
    void ShowOptionsMenu();

    /** Show the keybinding menu */
    UFUNCTION(BlueprintCallable, Category="Menu")
    void ShowKeybindMenu();

    /** Save current user settings to config */
    UFUNCTION(BlueprintCallable, Category="Menu")
    void SaveSettings();

protected:
    virtual void Deinitialize() override;

    UPROPERTY()
    UUserWidget* CurrentWidget = nullptr;

    UPROPERTY(EditDefaultsOnly, Category="Menu")
    TSubclassOf<UUserWidget> MainMenuClass;

    UPROPERTY(EditDefaultsOnly, Category="Menu")
    TSubclassOf<UUserWidget> OptionsMenuClass;

    UPROPERTY(EditDefaultsOnly, Category="Menu")
    TSubclassOf<UUserWidget> KeybindMenuClass;

    void ShowWidget(TSubclassOf<UUserWidget> WidgetClass);
};
