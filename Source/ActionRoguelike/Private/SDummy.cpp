// Fill out your copyright notice in the Description page of Project Settings.


#include "SDummy.h"

#include "SAttributeComponent.h"
#include "SMagicProjectile.h"

// Sets default values
ASDummy::ASDummy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComp"));
}

void ASDummy::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &ASDummy::OnHealthChanged);
	MeshComp->OnComponentHit.AddDynamic(this, &ASDummy::OnActorHit);
}

// Called every frame
void ASDummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASDummy::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
								  FVector NormalImpulse, const FHitResult& Hit)
{
	if (ASMagicProjectile* projectile = Cast<ASMagicProjectile>(OtherActor))
	{
		AttributeComp->ApplyHealthChange(-projectile->GetBaseDamage());
	}
	
}

void ASDummy::OnHealthChanged(AActor* IntigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (!MeshComp) return;

	// if (NewHealth < 0)
	// {
	// 	MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	// 	MeshComp->SetScalarParameterValueOnMaterials("HitFlashSpeedMultiplier", 4);
	// }
}


