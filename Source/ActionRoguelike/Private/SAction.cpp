// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"
#include "Net/UnrealNetwork.h"
#include "SActionComponent.h"


void USAction::Initialize(USActionComponent* NewActionComp)
{
	ActionComponent = NewActionComp;
}


bool USAction::CanStart(AActor* Instigator)
{
	if (RepData.bIsActionRunning)
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

	ActionComp->OnActionStart.Broadcast(ActionComp, this);

	if (GetOwningComponent()->GetOwnerRole() == ROLE_Authority)
	{
		TimeStarted = GetWorld()->TimeSeconds;
	}

	RepData.bIsActionRunning = true;
	RepData.Instigator = Instigator;

}

void USAction::StopAction(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("StopObj: %s"), *GetNameSafe(this));

	//ensureAlways(bIsActionRunning);

	USActionComponent* ActionComp = GetOwningComponent();
	ActionComp->ActiveGameplayTags.RemoveTags(GrantedTags);

	ActionComp->OnActionStop.Broadcast(ActionComp, this);

	RepData.bIsActionRunning = false;
	RepData.Instigator = Instigator;
}

bool USAction::IsSupportedForNetworking() const
{
	return true;
}

UWorld* USAction::GetWorld() const
{
	/** 
	 * Outer is set when creating Action via NewObject<T> in ActionComponent AddAction()
	 * Without this overridden implementation, GetWorld() node in Blueprint will not be accessible
	 */
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}
	
	return nullptr;
}

USActionComponent* USAction::GetOwningComponent() const
{
	return ActionComponent;
	
}

void USAction::OnRep_RepData()
{
	if (RepData.bIsActionRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}

bool USAction::IsActionRunning() const
{
	return RepData.bIsActionRunning;
}

FGameplayTagContainer USAction::GetGrantedTags() const
{
	return GrantedTags;
}

void USAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAction, RepData);
	DOREPLIFETIME(USAction, ActionComponent);
	DOREPLIFETIME(USAction, TimeStarted);
}