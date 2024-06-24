// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/HealthPotion.h"

#include "SAttributeComponent.h"

// Sets default values
AHealthPotion::AHealthPotion()
{
	HealthToAdd = 40;
	bCanRespawn = true;
	RespawnRate = 10.0f;
}

// Called when the game starts or when spawned
void AHealthPotion::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHealthPotion::ApplyEffect(USAttributeComponent* AttributeComp)
{
	Super::ApplyEffect(AttributeComp);
	AttributeComp->ApplyHealthChange(HealthToAdd);
}


