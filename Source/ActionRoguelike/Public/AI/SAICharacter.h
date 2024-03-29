// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class UPawnSensingComponent;
class USActionComponent;
class USAttributeComponent;
class USWorldUserWidget;
class UUserWidget;

UCLASS()
class ACTIONROGUELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAICharacter();

protected:
	
	virtual void PostInitializeComponents() override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PlayerSpottedWidgetClass;

	UPROPERTY()
	USWorldUserWidget* ActiveHealthBar;

	UPROPERTY()
	USWorldUserWidget* PlayerSpottedWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent* AttributeComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USActionComponent* ActionComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;
	
	AActor* GetTargetActor() const;

	void SetTargetActor(AActor* NewTarget);

	void RemoveWidget();

	UFUNCTION()
	void OnHealthChange(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewValue, float Delta);

	UFUNCTION()
	void OnSeePawn(APawn* Pawn);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastOnSeePawn();

};
