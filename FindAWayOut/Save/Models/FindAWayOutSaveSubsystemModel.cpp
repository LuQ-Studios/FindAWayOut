// Copyright (c) 2026, LuQ Studios. All Rights Reserved.


#include "FindAWayOutSaveSubsystemModel.h"

#include "FindAWayOut.h"
#include "Save/FindAWayOutSaveSubsystem.h"

UFindAWayOutSaveSubsystemModel::UFindAWayOutSaveSubsystemModel()
{
	bSerializeAllSaveGameProperties = false;
}

bool UFindAWayOutSaveSubsystemModel::OnPreSave()
{
	if (!Super::OnPreSave())
		return false;
	
	UFindAWayOutSaveSubsystem* subsystem = CastChecked<UFindAWayOutSaveSubsystem>(SaveObject);
	
	LoadPriority = TNumericLimits<int32>::Max(); // Make sure this is the first model that is loaded.
	WorldSaves = subsystem->WorldSaves;
	
	WorldSaveResult = EFindAWayOutSaveSubsystemModelWorldSaveResult::InProgress;
	if (!subsystem->SaveCurrentWorld(FPulseSaveManagerProcessedSignature::FDelegate::CreateUObject(this, &UFindAWayOutSaveSubsystemModel::WorldSaveComplete)))
	{
		UE_LOG(LogFindAWayOut, Warning, TEXT("UFindAWayOutSaveSubsystemModel::OnPreSave - Failed to save current world before saving save subsystem model."));
		return false;
	}
	
	return true;
}

bool UFindAWayOutSaveSubsystemModel::OnPostSave()
{
	if (WorldSaveResult == EFindAWayOutSaveSubsystemModelWorldSaveResult::Failure)
	{
		UE_LOG(LogFindAWayOut, Warning, TEXT("UFindAWayOutSaveSubsystemModel::OnPostSave - Failed to save current world. Aborting save."));
		return false;
	}
	
	return Super::OnPostSave();
}

bool UFindAWayOutSaveSubsystemModel::OnPreLoad()
{
	if (!Super::OnPreLoad())
		return false;
	
	UFindAWayOutSaveSubsystem* subsystem = CastChecked<UFindAWayOutSaveSubsystem>(SaveObject);
	subsystem->WorldSaves = WorldSaves;
	
	return true;
}

bool UFindAWayOutSaveSubsystemModel::OnPostLoad()
{
	return Super::OnPostLoad();
}

bool UFindAWayOutSaveSubsystemModel::IsReadyToSave() const
{
	return WorldSaveResult != EFindAWayOutSaveSubsystemModelWorldSaveResult::InProgress;
}

bool UFindAWayOutSaveSubsystemModel::IsReadyToLoad() const
{
	return true;
}

void UFindAWayOutSaveSubsystemModel::WorldSaveComplete(UPulseSaveManager* SaveManager, bool bSuccess)
{
	WorldSaveResult = bSuccess ? EFindAWayOutSaveSubsystemModelWorldSaveResult::Success : EFindAWayOutSaveSubsystemModelWorldSaveResult::Failure;
}
