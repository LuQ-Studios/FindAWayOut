// Copyright (c) 2026, LuQ Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FindAWayOutUITypes.h"
#include "PulseHUD/Public/HUD/PulseHUDModalMetadata.h"
#include "FindAWayOutModalMetadata.generated.h"

/**
 * 
 */
UCLASS()
class FINDAWAYOUT_API UFindAWayOutModalMetadata : public UPulseHUDModalMetadata
{
	GENERATED_BODY()
public:
	
	/** Type of the modal */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn), Category = "Modal")
	EFindAWayOutModalType ModalType = EFindAWayOutModalType::Default;
	
	/** Size of the modal */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn), Category = "Modal")
	EFindAWayOutModalSize ModalSize = EFindAWayOutModalSize::Medium;
};
