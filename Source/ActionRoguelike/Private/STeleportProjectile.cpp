// Fill out your copyright notice in the Description page of Project Settings.


#include "STeleportProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASTeleportProjectile::ASTeleportProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TeleportDelay = 0.2f;
	DetonateDelay = 0.2f;

	MovementComp->InitialSpeed = 6000.0f;
}

// Called when the game starts or when spawned
void ASTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_DelayedDetonate, this, &ASTeleportProjectile::Explode, DetonateDelay);
}

void ASTeleportProjectile::Explode_Implementation()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_DelayedDetonate);

	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

	EffectComp->DeactivateSystem();

	MovementComp->StopMovementImmediately();
	SetActorEnableCollision(false);

	FTimerHandle TimerHandle_DelayedTeleport;
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport, this, &ASTeleportProjectile::TeleportInstigator, TeleportDelay);
}

void ASTeleportProjectile::TeleportInstigator() const
{
	AActor* ProjectileInstigator = GetInstigator();
	if (ensure(ProjectileInstigator))
	{
		ProjectileInstigator->TeleportTo(GetActorLocation(), ProjectileInstigator->GetActorRotation(), false, false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Projectile %s does not have an Instigator. Cant teleport"), *GetNameSafe(this));
	}
}
