// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "GameFramework/Actor.h"
#include "STeleportProjectile.generated.h"

class USphereComponent;
class UParticleSystemComponent;
class UProjectileMovementComponent;

UCLASS()
class ACTIONROGUELIKE_API ASTeleportProjectile : public ASProjectileBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTeleportProjectile();

protected:
	UPROPERTY(EditDefaultsOnly, Category="Teleport")
	float TeleportDelay;

	UPROPERTY(EditDefaultsOnly, Category="Teleport")
	float DetonateDelay;

	// handle cancel timer if we already hit something
	FTimerHandle TimerHandle_DelayedDetonate;

	void TeleportInstigator() const;
	
	virtual void Explode_Implementation() override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
