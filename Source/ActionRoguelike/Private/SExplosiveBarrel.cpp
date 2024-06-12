// Fill out your copyright notice in the Description page of Project Settings.

#include "SExplosiveBarrel.h"

#include "SAttributeComponent.h"
#include "Components/StaticMeshComponent.h"

#include "SMagicProjectile.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetSimulatePhysics(true);
	RootComponent = StaticMesh;

	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->SetupAttachment(StaticMesh);

	ForceComp->SetAutoActivate(false);

	ForceComp->Radius = 750.0f;
	ForceComp->ImpulseStrength = 2500.0f;
	ForceComp->bImpulseVelChange = true;
}

// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	StaticMesh->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnActorHit);
}

// Called every frame
void ASExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASExplosiveBarrel::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Cast<ASMagicProjectile>(OtherActor))
	{
		ApplyDamageInArea();
		ForceComp->FireImpulse();
	}
}

void ASExplosiveBarrel::ApplyDamageInArea() const
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

	FVector ActorLocation = GetActorLocation();
	
	TArray<FHitResult> Hits;
	TArray<AActor*> AffectedActors;
	
	FCollisionShape Shape;
	Shape.SetSphere(ForceComp->Radius);

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, ActorLocation, ActorLocation, FQuat::Identity, ObjectQueryParams, Shape);
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	if (Hits.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Actor was affected by the explosion!"))
		return;
	}
	
	for (FHitResult& Hit : Hits)
	{
		if (USAttributeComponent* AttributeComp = Hit.GetActor()->GetComponentByClass<USAttributeComponent>())
		{
			if (AActor** CachedActor = AffectedActors.FindByKey(Hit.GetActor()))
			{
				continue;
			}
			
			AttributeComp->ApplyHealthChange(-30.0f);
			AffectedActors.Push(Hit.GetActor());
		}
	}

	DrawDebugSphere(GetWorld(), ActorLocation, ForceComp->Radius, 15, LineColor);
}

