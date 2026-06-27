// Copyright (c) 2026, LuQ Studios. All Rights Reserved.


#include "FindAWayOutWorldList.h"

#include "FindAWayOutWorldDefinition.h"
#include "Misc/DataValidation.h"

#define LOCTEXT_NAMESPACE "FindAWayOut"

#if WITH_EDITOR

EDataValidationResult UFindAWayOutWorldList::IsDataValid(class FDataValidationContext& Context) const
{
	EDataValidationResult result = Super::IsDataValid(Context);
	
	TSet<FGameplayTag> visitedWorldTags;
	for (const FFindAWayOutWorldEntry& entry : WorldList)
	{
		UFindAWayOutWorldDefinition* worldDefinition = entry.WorldDefinition.LoadSynchronous();
		if (!worldDefinition)
		{
			result = EDataValidationResult::Invalid;
			Context.AddError(LOCTEXT("InvalidWorldDefinitionReference", "Invalid world definition reference."));
			
			continue;
		}
		
		if (visitedWorldTags.Contains(worldDefinition->WorldTag))
		{
			result = EDataValidationResult::Invalid;
			Context.AddError(FText::Format(LOCTEXT("DuplicateWorldTag", "Duplicate world tag '{0}' found."), FText::FromName(worldDefinition->WorldTag.GetTagName())));
		}
		else
		{
			visitedWorldTags.Add(worldDefinition->WorldTag);
		}
		
		if (entry.World.IsNull())
		{
			result = EDataValidationResult::Invalid;
			Context.AddError(FText::Format(LOCTEXT("InvalidWorldReference", "Invalid world reference for tag '{0}'."), FText::FromName(worldDefinition->WorldTag.GetTagName())));
		}
	}
	
	return result;
}


#endif

const FFindAWayOutWorldEntry* UFindAWayOutWorldList::FindEntry(const FGameplayTag& InWorldTag) const
{
	for (const FFindAWayOutWorldEntry& entry : WorldList)
	{
		UFindAWayOutWorldDefinition* worldDefinition = entry.WorldDefinition.LoadSynchronous();
		if (worldDefinition && worldDefinition->WorldTag == InWorldTag)
			return &entry;
	}
	
	return nullptr;
}

#undef LOCTEXT_NAMESPACE