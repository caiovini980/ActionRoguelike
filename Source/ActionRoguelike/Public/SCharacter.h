// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USInteractionComponent;
class USAttributeComponent;
class UAnimMontage;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category="AttackSkills")
	TSubclassOf<AActor> BlackholeClass;
	
	UPROPERTY(EditAnywhere, Category="MovementSkills")
	TSubclassOf<AActor> TeleportClass;

	UPROPERTY(EditAnywhere, Category="Attack")
	UAnimMontage* AttackAnim;
	
	FTimerHandle TimerHandle_PrimaryAttack;
	FTimerHandle TimerHandle_BlackholeAttack;
	FTimerHandle TimerHandle_CastTeleport;

	float TimeToAttack = 0.2f;

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Attributes")
	USAttributeComponent* AttributeComp;
	

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	
	void MoveForward(float Value);
	void MoveRight(float Value);
	
	void PrimaryInteract();
	
	void PrimaryAttack();
	void BlackholeAttack();
	void CastTeleport();
	void BlackholeAttack_Timelapsed();
	void PrimaryAttack_TimeLapsed();
	void CastTeleport_Timelapsed();

	void ClassToSpawn(TSubclassOf<AActor>& ActorClass);
	
	UFUNCTION()
	void OnHealthChanged(AActor* IntigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
