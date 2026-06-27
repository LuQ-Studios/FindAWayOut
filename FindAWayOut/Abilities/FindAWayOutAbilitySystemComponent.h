// Copyright (c) 2026, LuQ Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "FindAWayOutAbilityTypes.h"
#include "FindAWayOutAbilitySystemComponent.generated.h"

USTRUCT()
struct FFindAWayOutAbilitySave
{
	GENERATED_BODY()
public:
	
	UPROPERTY()
	FString AbilityClassPath;
	
	UPROPERTY()
	int32 Level = 0;
	
	UPROPERTY()
	EFindAWayOutAbilityInputID InputID = EFindAWayOutAbilityInputID::None;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FINDAWAYOUT_API UFindAWayOutAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	
	FGameplayAbilitySpecHandle GiveAbility(const FFindAWayOutAbility& InAbility, bool bCanBeSaved = true);	
	
	bool SaveAbilities(TArray<FFindAWayOutAbilitySave>& OutAbilities);
	bool LoadAbilities(TConstArrayView<FFindAWayOutAbilitySave> InAbilities);
	
private:
	
	/* List of abilities that should not be saved */
	TSet<FGameplayAbilitySpecHandle> NonSavableAbilities;
};
