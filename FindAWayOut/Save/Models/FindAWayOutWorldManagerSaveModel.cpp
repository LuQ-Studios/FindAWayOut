// Copyright (c) 2026, LuQ Studios. All Rights Reserved.


#include "FindAWayOutWorldManagerSaveModel.h"

#include "FindAWayOutSettings.h"
#include "FindAWayOutWorldManager.h"

UFindAWayOutWorldManagerSaveModel::UFindAWayOutWorldManagerSaveModel()
{
	bSerializeAllSaveGameProperties = false;
}

bool UFindAWayOutWorldManagerSaveModel::InitializeNewModel()
{
	SavedWorldTag = UFindAWayOutSettings::Get().FirstGameWorldTag;
	return Super::InitializeNewModel();
}

bool UFindAWayOutWorldManagerSaveModel::OnPreSave()
{
	if (!Super::OnPreSave())
		return false;
	
	LoadPriority = TNumericLimits<int32>::Max() - 1; // Make sure this is the second model that is loaded, right after the save subsystem model.
	
	UFindAWayOutWorldManager* manager = CastChecked<UFindAWayOutWorldManager>(SaveObject);
	SavedWorldTag = manager->CurrentWorldTag;
	
	return true;
}

bool UFindAWayOutWorldManagerSaveModel::OnPreLoad()
{
	return Super::OnPreLoad();
}

bool UFindAWayOutWorldManagerSaveModel::OnPostLoad()
{
	return Super::OnPostLoad();
}

bool UFindAWayOutWorldManagerSaveModel::IsReadyToSave() const
{
	return true;
}