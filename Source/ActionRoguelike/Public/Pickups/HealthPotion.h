// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPotionBase.h"
#include "GameFramework/Actor.h"
#include "HealthPotion.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AHealthPotion : public ASPotionBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHealthPotion();

protected:
	float HealthToAdd;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void ApplyEffect(USAttributeComponent* AttributeComp) override;
};
