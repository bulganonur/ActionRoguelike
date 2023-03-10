// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"
#include "SActionComponent.h"

bool USAction::CanStart(AActor* Instigator)
{
	if (bIsActionRunning)
	{
		return false;
	}

	USActionComponent* ActionComp = GetOwningComponent();
	if (ActionComp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}

	return true;
}

void USAction::StartAction(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("StartObj: %s"), *GetNameSafe(this));

	USActionComponent* ActionComp = GetOwningComponent();
	ActionComp->ActiveGameplayTags.AppendTags(GrantedTags);

	bIsActionRunning = true;
}

void USAction::StopAction(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("StopObj: %s"), *GetNameSafe(this));

	ensureAlways(bIsActionRunning);

	USActionComponent* ActionComp = GetOwningComponent();
	ActionComp->ActiveGameplayTags.RemoveTags(GrantedTags);

	bIsActionRunning = false;
}


UWorld* USAction::GetWorld() const
{
	/** 
	 * Outer is set when creating Action via NewObject<T> in ActionComponent AddAction()
	 * Without this overridden implementation, GetWorld() node in Blueprint will not be accessible
	 */
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if (Comp)
	{
		return Comp->GetWorld();
	}
	
	return nullptr;
}

USActionComponent* USAction::GetOwningComponent() const
{
	return Cast<USActionComponent>(GetOuter());
	
}


bool USAction::IsActionRunning() const
{
	return bIsActionRunning;
}