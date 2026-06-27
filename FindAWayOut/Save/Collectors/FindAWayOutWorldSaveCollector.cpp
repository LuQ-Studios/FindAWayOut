// Copyright (c) 2026, LuQ Studios. All Rights Reserved.


#include "FindAWayOutWorldSaveCollector.h"

#include "EngineUtils.h"
#include "PulseSaveManager.h"
#include "Abilities/FindAWayOutInteractAbility.h"
#include "Interfaces/FindAWayOutSaveInterface.h"

bool UFindAWayOutWorldSaveCollector::ShouldCollectActor(AActor* InActor) const
{
	return Pulse::Interface::ObjectImplementsInterface<UFindAWayOutSaveInterface>(InActor) && !IFindAWayOutSaveInterface::Execute_ShouldExcludeFromWorldSave(InActor);
}

bool UFindAWayOutWorldSaveCollector::CollectActorForLoad_Implementation(AActor* InActor)
{
	return !IFindAWayOutSaveInterface::Execute_ShouldExcludeFromLoadCollection(InActor);
}
