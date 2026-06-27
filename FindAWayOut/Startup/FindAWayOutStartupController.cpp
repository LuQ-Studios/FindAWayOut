// Copyright (c) 2026, LuQ Studios. All Rights Reserved.


#include "FindAWayOutStartupController.h"

#include "FindAWayOutSettings.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/PulsePlayerHUDManager.h"
#include "UI/FindAWayOutHUDManager.h"

void AFindAWayOutStartupController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (bTravelling)
		return;
	
	bool bReadyToTravel = true;
	
	UFindAWayOutHUDManager* hudManager = UGameInstance::GetSubsystem<UFindAWayOutHUDManager>(GetGameInstance());
	if (!hudManager || !hudManager->IsHUDManagerInitialized())
		bReadyToTravel = false;
	
	UPulsePlayerHUDManager* playerHudManager = ULocalPlayer::GetSubsystem<UPulsePlayerHUDManager>(GetLocalPlayer());
	if (!playerHudManager || !playerHudManager->IsHUDManagerInitialized())
		bReadyToTravel = false;
	
	if (bReadyToTravel)
	{
		bTravelling = true;
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, UFindAWayOutSettings::Get().MainMenuLevel);
	}
}

bool AFindAWayOutStartupController::ShouldShowLoadingScreen(FString& OutReason) const
{
	OutReason = TEXT("Starting up the game.");
	return true;
}
