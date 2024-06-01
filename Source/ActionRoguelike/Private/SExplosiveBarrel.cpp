// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/StaticMeshComponent.h"
#include "SExplosiveBarrel.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	RootComponent = StaticMesh;
}

// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASExplosiveBarrel::Explode()
{
	TArray<FHitResult> OutHits;
	FCollisionShape CollisionSphere = FCollisionShape::MakeSphere(400.0f);
	FVector OriginLocation = GetActorLocation();
	DrawDebugSphere(GetWorld(), OriginLocation, CollisionSphere.GetSphereRadius(), 30, FColor::Cyan,
		true);

	bool IsHit = GetWorld()->SweepMultiByChannel(OutHits, OriginLocation, OriginLocation,FQuat::Identity,
		ECC_WorldDynamic, CollisionSphere);
	
	if (IsHit)
	{
		for (auto& Hit : OutHits)
		{
			if (UStaticMeshComponent* MeshComp = Hit.GetActor()->GetComponentByClass<UStaticMeshComponent>())
			{
				if (!MeshComp->IsSimulatingPhysics()) { return; }

				UE_LOG(LogTemp, Warning, TEXT("BOOOM"))
				MeshComp->AddRadialImpulse(OriginLocation, 400.0f, 2000.0f, RIF_Constant, true);
			}
		}
	}
}

// Called every frame
void ASExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

