// Copyright (c) 2026, LuQ Studios. All Rights Reserved.


#include "FindAWayOutGameSaveCollector.h"

#include "EngineUtils.h"
#include "FindAWayOutPlayerState.h"
#include "FindAWayOutWorldManager.h"
#include "Save/FindAWayOutSaveSubsystem.h"

bool UFindAWayOutGameSaveCollector::CollectSaveLoadObjects(TSet<UObject*>& OutObjects)
{
	UFindAWayOutSaveSubsystem* saveSubsystem = GetTypedOuter<UFindAWayOutSaveSubsystem>();
	check(saveSubsystem);
	
	OutObjects.Reserve(OutObjects.Num() + 2);
	
	OutObjects.Add(saveSubsystem);
	OutObjects.Add(&UFindAWayOutWorldManager::Get(saveSubsystem));
	
	return true;
}

bool UFindAWayOutGameSaveCollector::CollectLoadObjects_Implementation(TSet<UObject*>& OutLoadObjects)
{
	return CollectSaveLoadObjects(OutLoadObjects);
}

bool UFindAWayOutGameSaveCollector::CollectSaveObjects_Implementation(TSet<UObject*>& OutSaveObjects)
{
	CollectSaveLoadObjects(OutSaveObjects);
	
	for (TActorIterator<AFindAWayOutPlayerState> it(GetWorld()); it; ++it)
		OutSaveObjects.Add(*it);
	
	return true;
}
