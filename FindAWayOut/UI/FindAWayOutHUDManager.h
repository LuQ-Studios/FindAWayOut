// Copyright (c) 2026, LuQ Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PulseHUD/Public/Subsystems/PulseGlobalHUDManager.h"
#include "FindAWayOutHUDManager.generated.h"

/**
 * 
 */
UCLASS()
class FINDAWAYOUT_API UFindAWayOutHUDManager : public UPulseGlobalHUDManager
{
	GENERATED_BODY()
protected:
	
	virtual TSubclassOf<UPulseHUDModalContainer> GetDefaultModalContainerClass(const FPulseHUDModalPushBase& Params) const override;
};
