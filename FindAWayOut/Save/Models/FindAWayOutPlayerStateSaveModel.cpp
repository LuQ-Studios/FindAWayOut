// Copyright (c) 2026, LuQ Studios. All Rights Reserved.


#include "FindAWayOutPlayerStateSaveModel.h"

#include "FindAWayOut.h"
#include "FindAWayOutPlayerState.h"
#include "Components/PulseQuestManager.h"

struct FFindAWayOutPlayerStateSaveModelVersion
{
	enum Type
	{		
		QuestManagerSaveSerialization = 0,
		VersionPlusOne,
		LatestVersion = VersionPlusOne - 1
	};
	
	const static FGuid GUID;

	FFindAWayOutPlayerStateSaveModelVersion() = delete;
};

const FGuid FFindAWayOutPlayerStateSaveModelVersion::GUID(0x12345678, 0x1234ABCD, 0x5678EF01, 0x23456789);
FDevVersionRegistration GFindAWayOutPlayerStateSaveModelVersion(FFindAWayOutPlayerStateSaveModelVersion::GUID, FFindAWayOutPlayerStateSaveModelVersion::LatestVersion, TEXT("FindAWayOutPlayerStateSaveModelVersion"));

void UFindAWayOutPlayerStateSaveModel::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);
	
	Ar.UsingCustomVersion(FFindAWayOutPlayerStateSaveModelVersion::GUID);
	
	if (Ar.IsSaveGame() && Ar.CustomVer(FFindAWayOutPlayerStateSaveModelVersion::GUID) >= FFindAWayOutPlayerStateSaveModelVersion::QuestManagerSaveSerialization)
		Ar << QuestManagerSave;
}

bool UFindAWayOutPlayerStateSaveModel::OnPreSave()
{
	if (!Super::OnPreSave())
		return false;
	
	bHadSaveError = false;
	
	AFindAWayOutPlayerState* playerState = CastChecked<AFindAWayOutPlayerState>(SaveObject);
	QuestManagerSavedHandle = playerState->QuestManager->OnSavedEvent().AddUObject(this, &UFindAWayOutPlayerStateSaveModel::OnQuestManagerSaved);
	
	if (!playerState->QuestManager->SaveQuestManager(QuestManagerSave))
	{
		UE_LOG(LogFindAWayOut, Error, TEXT("Failed to save quest manager for player state %s."), *playerState->GetName());
		return false;
	}
	
	bIsSavingQuestManager = true;
	return true;
}

bool UFindAWayOutPlayerStateSaveModel::OnPostSave()
{
	AFindAWayOutPlayerState* playerState = CastChecked<AFindAWayOutPlayerState>(SaveObject);
	
	playerState->QuestManager->OnSavedEvent().Remove(QuestManagerSavedHandle);
	QuestManagerSavedHandle.Reset();
	
	return Super::OnPostSave() && !bHadSaveError;
}

bool UFindAWayOutPlayerStateSaveModel::IsReadyToSave() const
{
	return !bIsSavingQuestManager;
}

bool UFindAWayOutPlayerStateSaveModel::OnPreLoad()
{
	return Super::OnPreLoad();
}

bool UFindAWayOutPlayerStateSaveModel::OnPostLoad()
{
	if (!Super::OnPostLoad())
		return false;
	
	AFindAWayOutPlayerState* playerState = CastChecked<AFindAWayOutPlayerState>(SaveObject);
	if (!playerState->QuestManager->LoadQuestManager(QuestManagerSave))
		return false;
	
	return true;
}

bool UFindAWayOutPlayerStateSaveModel::IsReadyToLoad() const
{
	return true;
}

void UFindAWayOutPlayerStateSaveModel::OnQuestManagerSaved(const TScriptInterface<IPulseQuestManagerInterface>& QuestManager, bool bSuccess)
{
	bIsSavingQuestManager = false;
	bHadSaveError |= !bSuccess;
}
