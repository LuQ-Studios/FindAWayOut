// Copyright (c) 2026, LuQ Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LoadingProcessInterface.h"
#include "GameFramework/PlayerController.h"
#include "FindAWayOutStartupController.generated.h"

/**
 * 
 */
UCLASS()
class FINDAWAYOUT_API AFindAWayOutStartupController : public APlayerController, public ILoadingProcessInterface
{
	GENERATED_BODY()
public:
	
	virtual void Tick(float DeltaSeconds) override;
	virtual bool ShouldShowLoadingScreen(FString& OutReason) const override;
	
private:
	
	bool bTravelling = false;
};
