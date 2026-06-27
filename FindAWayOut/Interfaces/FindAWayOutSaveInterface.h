// Copyright (c) 2026, LuQ Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/PulseSaveInterface.h"
#include "UObject/Interface.h"
#include "FindAWayOutSaveInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UFindAWayOutSaveInterface : public UPulseSaveInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FINDAWAYOUT_API IFindAWayOutSaveInterface : public IPulseSaveInterface
{
	GENERATED_BODY()
public:
	
	/** 
	 * Whether this should not be saved in the world save game.
	 * Useful for actors that exist in the world but should not be saved within the world save game such as a player controller.
	 * If this returns true you are manually responsible for saving and loading this actor in some other way.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Find A Way Out|Save")
	bool ShouldExcludeFromWorldSave() const;
	
	/**
	 * Whether this should be excluded from being collected as a load object by the save manager.
	 * If this returns true, the object is responsible for loading itself and will not be collected as a load object by the save manager.
	 * @returns true if this should be excluded from being collected as a load object, false otherwise.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Find A Way Out|Save")
	bool ShouldExcludeFromLoadCollection() const;
};
