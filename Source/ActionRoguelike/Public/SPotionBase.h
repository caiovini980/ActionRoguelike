// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SPotionBase.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UParticleSystemComponent;
class USAttributeComponent;

UCLASS(Abstract)
class ACTIONROGUELIKE_API ASPotionBase : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPotionBase();

protected:
	UPROPERTY(VisibleAnywhere, Category="Components")
	UStaticMeshComponent* MeshComp;
	UPROPERTY(VisibleAnywhere, Category="Components")
	UParticleSystemComponent* EffectsComp;
	UPROPERTY(VisibleAnywhere, Category="Components")
	USphereComponent* SphereComp;

	UPROPERTY(EditAnywhere, Category="Potion Settings")
	bool bCanRespawn;
	UPROPERTY(EditAnywhere, Category="Potion Settings")
	bool bCanInteractWith;
	UPROPERTY(EditAnywhere, Category="Potion Settings")
	float RespawnRate;

	FTimerHandle TimerHandle_TimeToRespawn;

	void EnablePotion();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	
	virtual void ApplyEffect(USAttributeComponent* AttributeComp);
	
	UFUNCTION()
	void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
