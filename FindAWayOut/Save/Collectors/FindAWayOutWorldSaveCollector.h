// Copyright (c) 2026, LuQ Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PulseSaveCollectorBase.h"
#include "Collectors/PulseSaveActorCollector.h"
#include "FindAWayOutWorldSaveCollector.generated.h"

/**
 * 
 */
UCLASS()
class FINDAWAYOUT_API UFindAWayOutWorldSaveCollector : public UPulseSaveActorCollector
{
	GENERATED_BODY()
public:
	
	virtual bool ShouldCollectActor(AActor* InActor) const override;
	virtual bool CollectActorForLoad_Implementation(AActor* InActor) override;
};
