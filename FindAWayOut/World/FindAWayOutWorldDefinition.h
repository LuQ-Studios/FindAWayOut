// Copyright (c) 2026, LuQ Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "FindAWayOutWorldDefinition.generated.h"

/**
 * 
 */
UCLASS()
class FINDAWAYOUT_API UFindAWayOutWorldDefinition : public UDataAsset
{
	GENERATED_BODY()
public:
	
	/** Tag of the world */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "World")
	FGameplayTag WorldTag;
};
