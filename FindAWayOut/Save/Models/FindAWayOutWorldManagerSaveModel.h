// Copyright (c) 2026, LuQ Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PulseSaveManager.h"
#include "PulseSaveModel.h"
#include "FindAWayOutWorldManagerSaveModel.generated.h"

/**
 * 
 */
UCLASS()
class FINDAWAYOUT_API UFindAWayOutWorldManagerSaveModel : public UPulseSaveModel
{
	GENERATED_BODY()
public:
	
	UFindAWayOutWorldManagerSaveModel();
	
public:
	
	virtual bool InitializeNewModel() override;
	virtual bool OnPreSave() override;
	virtual bool OnPreLoad() override;
	virtual bool OnPostLoad() override;
	virtual bool IsReadyToSave() const override;
	virtual bool ShouldBlockProcess() const override { return GetOuterUPulseSaveManager()->GetProcessState() == EPulseSaveManagerProcessType::Loading; }
	
public:
	
	UPROPERTY(SaveGame)
	FGameplayTag SavedWorldTag;
};
