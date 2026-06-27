// Copyright (c) 2026, LuQ Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PulseSaveModel.h"
#include "Abilities/FindAWayOutAbilitySystemComponent.h"
#include "Quest/Save/PulseQuestSaveTypes.h"
#include "FindAWayOutPlayerStateSaveModel.generated.h"

class IPulseQuestManagerInterface;
class IPulseQuestSaveInterface;
/**
 * 
 */
UCLASS()
class FINDAWAYOUT_API UFindAWayOutPlayerStateSaveModel : public UPulseSaveModel
{
	GENERATED_BODY()
public:
	
	virtual bool IsPersistent() const override { return true; }
	
	virtual void Serialize(FArchive& Ar) override;
	virtual bool OnPreSave() override;
	virtual bool OnPostSave() override;
	virtual bool IsReadyToSave() const override;
	
	virtual bool OnPreLoad() override;
	virtual bool OnPostLoad() override;
	virtual bool IsReadyToLoad() const override;
	
private:
	
	FDelegateHandle QuestManagerSavedHandle;
	void OnQuestManagerSaved(const TScriptInterface<IPulseQuestManagerInterface>& QuestManager, bool bSuccess);

private:
	
	/** Whether there was an error during save */
	bool bHadSaveError = false;
	
	/** Whether the quest manager is currently being saved */
	bool bIsSavingQuestManager = false;
	
public:
	
	FPulseQuestSaveGame QuestManagerSave;
};
