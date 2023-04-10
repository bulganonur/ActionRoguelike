// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"


//DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, USAttributeComponent*, OwningComp, float, NewHealth, float, Delta);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnRageChanged, AActor*, InstigatorActor, USAttributeComponent*, OwningComp, float, NewRage, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChange, AActor*, InstigatorActor, USAttributeComponent*, OwningComp, float, NewValue, float, Delta);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USAttributeComponent();

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static USAttributeComponent* GetAttributeComp(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static bool IsActorAlive(AActor* Actor);

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Attributes")
	float HealthMax;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float Rage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Attributes")
	float RageMax;

	/** Could mark as unreliable once we moved the 'state' out of SCharacter (e.g. once it's cosmetic only) */
	UFUNCTION(NetMulticast, Reliable)
	void MulticastOnHealthChange(AActor* InstigatorActor, float NewHealth, float Delta);

	/** Used for cosmetic changes only */
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastOnRageChange(AActor* InstigatorActor, float NewRage, float Delta);

public:

	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnAttributeChange OnHealthChange;

	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnAttributeChange OnRageChange;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;

	bool IsFullHealth() const;

	float GetHealth() const;
	float GetHealthMax() const;

	float GetRage() const;
	float GetRageMax() const;
	void SetRage(float Delta);

	bool Kill(AActor* InstigatorActor);
};
