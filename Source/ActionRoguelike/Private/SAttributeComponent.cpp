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

	// clamp health
	FMath::Clamp(Health, 0, HealthMax);

	// Hit flash when damaged only
	if (Delta < 0)
	{
		if (UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass())))
		{
			UE_LOG(LogTemp, Warning, TEXT("Changing health from %s by %f"), *GetNameSafe(GetOwner()), Delta)
			MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
			MeshComp->SetScalarParameterValueOnMaterials("HitFlashSpeedMultiplier", 4);
		}
	}
	
	if (Health == 0)
	{
		// owner died
	}

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	
	return true;
}

