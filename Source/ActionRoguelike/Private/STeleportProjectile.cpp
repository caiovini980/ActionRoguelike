// Fill out your copyright notice in the Description page of Project Settings.


#include "STeleportProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASTeleportProjectile::ASTeleportProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");
	RootComponent = SphereComp;
	
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
	
	ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	ParticleComp->SetupAttachment(SphereComp);
}

// Called when the game starts or when spawned
void ASTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void ASTeleportProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (APawn* ProjectileInstigator = GetInstigator())
	{
		SphereComp->IgnoreActorWhenMoving(ProjectileInstigator, true);
		ProjectileInstigator->MoveIgnoreActorAdd(this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Projectile %s does not have an Instigator."), *GetNameSafe(this));
	}
	
	GetWorldTimerManager().SetTimer(TeleportProjectileTravelTimer, this, &ASTeleportProjectile::EndTeleportTravel, 1.f);
}

void ASTeleportProjectile::EndTeleportTravel()
{
	OnTeleportProjectileFinishTravel();
	GetWorldTimerManager().SetTimer(TeleportPlayerTimer, this, &ASTeleportProjectile::TeleportInstigator, 0.5f);
}

void ASTeleportProjectile::TeleportInstigator()
{
	if (APawn* ProjectileInstigator = GetInstigator())
	{
		ProjectileInstigator->TeleportTo(GetActorLocation(), GetActorRotation());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Projectile %s does not have an Instigator. Cant teleport"), *GetNameSafe(this));
	}
	
	Destroy();
}

// Called every frame
void ASTeleportProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// EVENTS
void ASTeleportProjectile::OnTeleportProjectileFinishTravel_Implementation()
{}

