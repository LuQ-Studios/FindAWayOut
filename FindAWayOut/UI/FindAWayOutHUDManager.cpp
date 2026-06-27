// Copyright (c) 2026, LuQ Studios. All Rights Reserved.


#include "FindAWayOutHUDManager.h"

#include "FindAWayOutModalMetadata.h"
#include "FindAWayOutSettings.h"
#include "Components/Modal/PulseHUDModalContainer.h"

TSubclassOf<UPulseHUDModalContainer> UFindAWayOutHUDManager::GetDefaultModalContainerClass(const FPulseHUDModalPushBase& Params) const
{
	if (UFindAWayOutModalMetadata* metadata = Cast<UFindAWayOutModalMetadata>(Params.ModalMetadata))
	{
		TSoftClassPtr<UPulseHUDModalContainer>* containerClassPtr = UFindAWayOutSettings::Get().ModalSizeToContainerMap.Find(metadata->ModalSize);
		if (UClass* containerClass = containerClassPtr ? containerClassPtr->LoadSynchronous() : nullptr)
			return containerClass;
	}
	
	return Super::GetDefaultModalContainerClass(Params);
}
