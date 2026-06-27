// Copyright (c) 2026, LuQ Studios. All Rights Reserved.


#include "FindAWayOutCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FindAWayOutInputData.h"
#include "Abilities/Interact/PulseInteractAbility.h"

namespace FindAWayOut::Character
{
	UE_DEFINE_GAMEPLAY_TAG(Character_01, "FindAWayOut.Character.1");
	UE_DEFINE_GAMEPLAY_TAG(Character_02, "FindAWayOut.Character.2");
}

// Sets default values
AFindAWayOutCharacter::AFindAWayOutCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	AbilitySystemComponent = CreateDefaultSubobject<UFindAWayOutAbilitySystemComponent>("AbilitySystem");
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	AbilitySystemComponent->GenericConfirmInputID = static_cast<int32>(EFindAWayOutAbilityInputID::Confirm);
	AbilitySystemComponent->GenericCancelInputID = static_cast<int32>(EFindAWayOutAbilityInputID::Cancel);
	
	// ConstructorHelpers::FObjectFinder<UFindAWayOutCharacterInputData> inputData(TEXT("/Game/Content/Input/DA_CharacterInput.DA_CharacterInput"));
	// if (inputData.Succeeded())
	// 	CharacterInputData = inputData.Object;
}

// Called when the game starts or when spawned
void AFindAWayOutCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		for (const FFindAWayOutAbility& ability : InitialAbilities)
			AbilitySystemComponent->GiveAbility(ability, false); // We don't want to save initial abilities
	}
}

void AFindAWayOutCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UFindAWayOutCharacterInputData* data = CharacterInputData.LoadSynchronous())
		data->RemoveInputContexts(Pulse::Utils::GetLocalPlayer(this));
	
	Super::EndPlay(EndPlayReason);
}

UAbilitySystemComponent* AFindAWayOutCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}


void AFindAWayOutCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AFindAWayOutCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UFindAWayOutCharacterInputData* data = CharacterInputData.LoadSynchronous();
	if (ensureMsgf(data, TEXT("Character input data is not set on character %s Please set it to properly bind character input."), *GetName()))
	{
		UEnhancedInputComponent* component = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
		
		component->BindAction(data->MoveAction, ETriggerEvent::Triggered, this, &AFindAWayOutCharacter::MoveAction);
		component->BindAction(data->LookAction, ETriggerEvent::Triggered, this, &AFindAWayOutCharacter::LookAction);
		component->BindAction(data->InteractAction, ETriggerEvent::Triggered, this, &AFindAWayOutCharacter::InteractAction);
		
		const bool bAppliedContexts = data->ApplyInputContexts(Pulse::Utils::GetLocalPlayer(this));
		ensureMsgf(bAppliedContexts, TEXT("Failed to apply input contexts for character %s. Please check the input contexts set on the character input data."), *GetName());
	}
}

FName AFindAWayOutCharacter::GetUniqueSaveIdentifier_Implementation() const
{
	return CharacterIdentifier.GetTagName();
}

void AFindAWayOutCharacter::MoveAction(const FInputActionValue& Value)
{
	const FVector2D moveVector = Value.Get<FVector2D>();
	FRotationMatrix rotationMatrix(FRotator(0., GetControlRotation().Yaw, 0.));
	
	AddMovementInput(rotationMatrix.GetUnitAxis(EAxis::X), moveVector.Y);
	AddMovementInput(rotationMatrix.GetUnitAxis(EAxis::Y), moveVector.X);
}

void AFindAWayOutCharacter::LookAction(const FInputActionValue& Value)
{
	const FVector2D lookVector = Value.Get<FVector2D>();
	
	AddControllerPitchInput(lookVector.Y);
	AddControllerYawInput(lookVector.X);
}

void AFindAWayOutCharacter::InteractAction(const FInputActionValue& Value)
{	
	if (Value.Get<bool>()) // If pressed
		GetAbilitySystemComponent()->HandleGameplayEvent(Pulse::Ability::Interact::TAG_Pulse_Gameplay_Event_Interact_Started_Trigger, nullptr);
	else
		GetAbilitySystemComponent()->HandleGameplayEvent(Pulse::Ability::Interact::TAG_Pulse_Gameplay_Event_Interact_Ended_Trigger, nullptr);
}