// Copyright (c) 2026, LuQ Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "FindAWayOutWorldSettings.generated.h"

class UFindAWayOutWorldDefinition;

/**
 * 
 */
UCLASS()
class FINDAWAYOUT_API AFindAWayOutWorldSettings : public AWorldSettings
{
	GENERATED_BODY()
public:
	
	static AFindAWayOutWorldSettings* Get(const UObject* WorldContextObject);
	
public:
	
	/** Definition of the world if this is a game world */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Find A Way Out")
	TSoftObjectPtr<UFindAWayOutWorldDefinition> WorldDefinition;
};
