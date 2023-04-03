// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * Include neeeded here in the header file
 * because the FGameplayTagContainer(struct) GrantTags/BlockedTags is not declared as a pointer and compiler needs to know its size
 */
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "SAction.generated.h"


class USActionComponent;


UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()

public:

	void Initialize(USActionComponent* NewActionComp);
	
	UFUNCTION(BlueprintCallable, Category = "Action")
	virtual bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Action")
	virtual void StartAction(AActor* Instigator);
	
	UFUNCTION(BlueprintCallable, Category = "Action")
	virtual void StopAction(AActor* Instigator);

	bool IsSupportedForNetworking() const override;

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsActionRunning() const;

	FGameplayTagContainer GetGrantedTags() const;

	UWorld* GetWorld() const override;

	/** Action nickname to start/stop without a reference to the object */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	/** Start immediately when added to an action component */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bIsAutoStartOn;

protected:

	UPROPERTY(Replicated)
	USActionComponent* ActionComponent;

	UFUNCTION(BlueprintCallable, Category = "Action")
	USActionComponent* GetOwningComponent() const;

	/** Tags added to owning actor when activated, removed when action stops */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantedTags;

	/** Action can only start if OwningActor has none of these Tags applied */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	UPROPERTY(ReplicatedUsing = "OnRep_IsActionRunning")
	bool bIsActionRunning;

	UFUNCTION()
	void OnRep_IsActionRunning();
};
