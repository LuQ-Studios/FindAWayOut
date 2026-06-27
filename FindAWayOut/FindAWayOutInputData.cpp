// Copyright (c) 2026, LuQ Studios. All Rights Reserved.


#include "FindAWayOutInputData.h"

#include "EnhancedInputSubsystems.h"

bool UFindAWayOutInputDataBase::ApplyInputContexts(ULocalPlayer* InPlayer) const
{	
	if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(InPlayer))
	{
		bool bbFailedAny = false;
		
		for (const FFindAWayOutInputContext& context : InputContexts)
		{
			if (UInputMappingContext* loadedContext = context.MappingContext.LoadSynchronous())
				subsystem->AddMappingContext(loadedContext, context.Priority);
			else
				bbFailedAny = true;
		}
		
		return !bbFailedAny;
	}
		
	return false;
}

bool UFindAWayOutInputDataBase::RemoveInputContexts(ULocalPlayer* InPlayer) const
{
	if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(InPlayer))
	{
		bool bbFailedAny = false;
		
		for (const FFindAWayOutInputContext& context : InputContexts)
		{
			if (UInputMappingContext* loadedContext = context.MappingContext.LoadSynchronous())
				subsystem->RemoveMappingContext(loadedContext);
			else
				bbFailedAny = true;
		}
		
		return !bbFailedAny;
	}
	
	return false;
}