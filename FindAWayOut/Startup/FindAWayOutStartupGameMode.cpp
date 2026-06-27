// Copyright (c) 2026, LuQ Studios. All Rights Reserved.


#include "FindAWayOutStartupGameMode.h"

#include "FindAWayOutStartupController.h"

AFindAWayOutStartupGameMode::AFindAWayOutStartupGameMode()
{
	DefaultPawnClass = nullptr;
	PlayerControllerClass = AFindAWayOutStartupController::StaticClass();
}
