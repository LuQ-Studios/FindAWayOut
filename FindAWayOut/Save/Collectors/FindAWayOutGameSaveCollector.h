// Copyright (c) 2026, LuQ Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PulseSaveCollectorBase.h"
#include "Collectors/PulseSaveRegisterCollector.h"
#include "FindAWayOutGameSaveCollector.generated.h"

/**
 * 
 */
UCLASS()
class FINDAWAYOUT_API UFindAWayOutGameSaveCollector : public UPulseSaveRegisterCollector
{
	GENERATED_BODY()
public:
	
	bool CollectSaveLoadObjects(TSet<UObject*>& OutObjects);
	
	virtual bool CollectSaveObjects_Implementation(TSet<UObject*>& OutSaveObjects) override;
	virtual bool CollectLoadObjects_Implementation(TSet<UObject*>& OutLoadObjects) override;
};
