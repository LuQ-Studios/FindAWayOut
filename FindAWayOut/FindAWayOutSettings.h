// Copyright (c) 2026, LuQ Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "UI/FindAWayOutUITypes.h"
#include "UObject/SoftObjectPtr.h"
#include "FindAWayOutSettings.generated.h"

class UPulseHUDModalContainer;
class UUserWidget;
class UFindAWayOutWorldList;
/**
 * 
 */
UCLASS(config = Game, defaultconfig, meta = (DisplayName = "Find A Way Out"))
class FINDAWAYOUT_API UFindAWayOutSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	
	static UFindAWayOutSettings& Get();
	
public:
	
	/** Pause menu widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, Category = "UI")
	TSoftClassPtr<UUserWidget> PauseMenuWidgetClass;
	
public:
	
	/** Main menu level */
	UPROPERTY(EditAnywhere, Config, Category = "World")
	TSoftObjectPtr<UWorld> MainMenuLevel;
	
	/** First game world to load when starting a new game. */
	UPROPERTY(EditAnywhere, Config, Category = "World")
	FGameplayTag FirstGameWorldTag;
	
	/** World list data table */
	UPROPERTY(EditAnywhere, Config, Category = "World")
	TSoftObjectPtr<UFindAWayOutWorldList> WorldList;
	
public:
	
	/** Modal sizes mapped to modal containers */
	UPROPERTY(EditAnywhere, Config, Category = "UI|Modal")
	TMap<EFindAWayOutModalSize, TSoftClassPtr<UPulseHUDModalContainer>> ModalSizeToContainerMap;
};
