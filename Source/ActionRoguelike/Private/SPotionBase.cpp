// Fill out your copyright notice in the Description page of Project Settings.


#include "SPotionBase.h"

#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASPotionBase::ASPotionBase()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetupAttachment(MeshComp);

	EffectsComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EffectsComp"));
	EffectsComp->SetupAttachment(MeshComp);

	bCanRespawn = false;
	bCanInteractWith = true;
	RespawnRate = 1.0f;
}

// Called when the game starts or when spawned
void ASPotionBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASPotionBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASPotionBase::OnActorBeginOverlap);
}

void ASPotionBase::Interact_Implementation(APawn* InstigatorPawn)
{
	bCanInteractWith = false;
	this->SetActorHiddenInGame(true);
	
	// Start timer to respawn if possible
	if (bCanRespawn)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_TimeToRespawn, this, &ASPotionBase::EnablePotion, RespawnRate);
	}
}

void ASPotionBase::EnablePotion()
{
	bCanInteractWith = true;
	this->SetActorHiddenInGame(false);
}

void ASPotionBase::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (!bCanInteractWith) return;
	
	// execute effect if player
	if (Cast<ASCharacter>(OtherActor))
	{
		if (USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass())))
		{
			APawn* CastedPawn = Cast<APawn>(OtherActor);
			
			Interact_Implementation(CastedPawn);
			ApplyEffect(AttributeComp);
		}
	}
}

void ASPotionBase::ApplyEffect(USAttributeComponent* AttributeComp) {}
