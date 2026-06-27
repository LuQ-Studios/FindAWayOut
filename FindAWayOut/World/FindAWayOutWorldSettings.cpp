// Copyright (c) 2026, LuQ Studios. All Rights Reserved.


#include "FindAWayOutWorldSettings.h"

AFindAWayOutWorldSettings* AFindAWayOutWorldSettings::Get(const UObject* WorldContextObject)
{
	if (UWorld* world = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		return Cast<AFindAWayOutWorldSettings>(world->GetWorldSettings());
	
	return nullptr;
}
