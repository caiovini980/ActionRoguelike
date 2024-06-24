// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	HealthMax = 100.0f;
	Health = HealthMax;

	// ...
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;

	// Hit flash when damaged only
	if (Delta < 0)
	{
		if (UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass())))
		{
			MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
			MeshComp->SetScalarParameterValueOnMaterials("HitFlashSpeedMultiplier", 4);
		}
	}

	// Clamp health values
	if (Health <= 0)
	{
		Health = 0;
		// owner died
	}
	else if (Health >= HealthMax)
	{
		Health = HealthMax;
	}

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	
	return true;
}

