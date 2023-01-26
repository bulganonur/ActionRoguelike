// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USAttributeComponent;
class USInteractionComponent;
class USpringArmComponent;


UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> MagicProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> DarkholeProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> DashProjectileClass;

	UPROPERTY()
	AActor* DashObjPtr;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* VFX_TeleportIn;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* VFX_TeleportOut;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* VFX_PortalFX;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	FTimerHandle TimerHandle_PrimaryAttack;

	FHitResult Hit;
	FActorSpawnParameters SpawnParams;

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent* AttributeComp;


	void MoveForward(float Value);
	void MoveRight(float Value);
	
	FTransform CalculateTransform();

	void PrimaryAttack();
	void PrimaryAttack_TimeElapsed();

	void UltimateAttack();
	void UltimateAttack_TimeElapsed();

	void DashAbility();
	void DashAbility_TimeElapsed_SpawnProjectile();
	void DashAbility_TimeElapsed_Teleport();

	void PrimaryInteract();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
