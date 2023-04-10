// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SAnimInstance.generated.h"


class USActionComponent;


UCLASS()
class ACTIONROGUELIKE_API USAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	USActionComponent* ActionComp;

	/** Is pawn stunned based on GameplayTag data */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	bool bIsStunned;

	void NativeInitializeAnimation() override;

	void NativeUpdateAnimation(float DeltaSeconds) override;
};
