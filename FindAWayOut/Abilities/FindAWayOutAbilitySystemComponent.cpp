// Copyright (c) 2026, LuQ Studios. All Rights Reserved.


#include "FindAWayOutAbilitySystemComponent.h"

#include "FindAWayOut.h"

FGameplayAbilitySpecHandle UFindAWayOutAbilitySystemComponent::GiveAbility(const FFindAWayOutAbility& InAbility, bool bCanBeSaved)
{
	if (UClass* cl = InAbility.Ability.LoadSynchronous())
	{
		FGameplayAbilitySpecHandle handle = UAbilitySystemComponent::GiveAbility(FGameplayAbilitySpec(cl, InAbility.Level, InAbility.InputID == EFindAWayOutAbilityInputID::None ? INDEX_NONE : static_cast<int32>(InAbility.InputID)));
		if (!handle.IsValid())
			return handle;
		
		if (!bCanBeSaved)
			NonSavableAbilities.Add(handle);
		
		return handle;
	}
	
	return FGameplayAbilitySpecHandle();
}

bool UFindAWayOutAbilitySystemComponent::SaveAbilities(TArray<FFindAWayOutAbilitySave>& OutAbilities)
{
	OutAbilities.Reset(ActivatableAbilities.Items.Num());
	
	for (const FGameplayAbilitySpec& spec : ActivatableAbilities.Items)
	{
		if (NonSavableAbilities.Contains(spec.Handle))
			continue;
		
		FFindAWayOutAbilitySave& save = OutAbilities.AddDefaulted_GetRef();
		
		save.AbilityClassPath = spec.Ability->GetClass()->GetPathName();
		save.Level = spec.Level;
		save.InputID = static_cast<EFindAWayOutAbilityInputID>(spec.InputID);
	}
	
	return true;
}

bool UFindAWayOutAbilitySystemComponent::LoadAbilities(TConstArrayView<FFindAWayOutAbilitySave> InAbilities)
{
	bool bSuccess = true;
	
	for (const FFindAWayOutAbilitySave& ability : InAbilities)
	{
		UClass* abilityClass = UClass::TryFindTypeSlow<UClass>(ability.AbilityClassPath);
		if (!abilityClass)
		{
			abilityClass = LoadObject<UClass>(nullptr, *ability.AbilityClassPath);
			if (!abilityClass)
			{
				UE_LOG(LogFindAWayOut, Warning, TEXT("Failed to load ability class from path %s. Skipping loading of this ability."), *ability.AbilityClassPath);
				bSuccess = false;
				
				continue;
			}
		}
		
		FGameplayAbilitySpecHandle handle = Super::GiveAbility(FGameplayAbilitySpec(abilityClass, ability.Level, ability.InputID == EFindAWayOutAbilityInputID::None ? INDEX_NONE : static_cast<int32>(ability.InputID)));
		if (!handle.IsValid())
		{
			UE_LOG(LogFindAWayOut, Warning, TEXT("Failed to give ability of class %s. Skipping loading of this ability."), *GetNameSafe(abilityClass));
			bSuccess = false;
			
			continue;
		}
	}
	
	return bSuccess;
}
