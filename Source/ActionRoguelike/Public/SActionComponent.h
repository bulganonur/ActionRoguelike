// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

/** 
 * Include neeeded here in the header file
 * because the FGameplayTagContainer(struct) ActiveGameplayTags is not declared as a pointer and compiler needs to know its size
 */ 
#include "GameplayTagContainer.h" 
#include "SActionComponent.generated.h"

class USAction;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	USActionComponent();

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void AddAction(TSubclassOf<USAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StartActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StopActionByName(AActor* Instigator, FName ActionName);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<USAction*> Actions;

	/** Grant actions at game start **/
	UPROPERTY(EditAnywhere, Category = "Actions")
	TArray<TSubclassOf<USAction>> DefaultActions;

public:	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
