// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"

void USAction::StartAction(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("StartObj: %s"), *GetNameSafe(this));
}

void USAction::StopAction(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("StopObj: %s"), *GetNameSafe(this));
}

UWorld* USAction::GetWorld() const
{
	/** 
	 * Outer is set when creating Action via NewObject<T>
	 * Without this overridden implementation, GetWorld() node in Blueprint will not be accessible
	 */
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if (Comp)
	{
		return Comp->GetWorld();
	}
	
	return nullptr;
}
