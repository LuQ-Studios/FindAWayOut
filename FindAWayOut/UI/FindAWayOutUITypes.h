// Copyright (c) 2026, LuQ Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FindAWayOutUITypes.generated.h"

UENUM(BlueprintType)
enum class EFindAWayOutModalType : uint8
{
	Default,
	
};

UENUM(BlueprintType)
enum class EFindAWayOutModalSize : uint8
{
	Small,
	Medium,
	Large
};