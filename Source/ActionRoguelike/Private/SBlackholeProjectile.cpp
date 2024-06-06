// Fill out your copyright notice in the Description page of Project Settings.


#include "SBlackholeProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ASBlackholeProjectile::ASBlackholeProjectile()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetCollisionProfileName("Blackhole");
	RootComponent = SphereComp;
	
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	MovementComp->InitialSpeed = 500.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComp->Radius = 500.0f;
	RadialForceComp->ImpulseStrength = -1500.0f;
	RadialForceComp->bIgnoreOwningActor = true;
	RadialForceComp->bImpulseVelChange = true;

	RadialForceComp->SetAutoActivate(false);
	RadialForceComp->SetupAttachment(SphereComp);

	ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
	ParticleComp->SetupAttachment(SphereComp);
	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void ASBlackholeProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASBlackholeProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	if (APawn* BlackholeInstigator = GetInstigator())
	{
		SphereComp->IgnoreActorWhenMoving(BlackholeInstigator, true);
		BlackholeInstigator->MoveIgnoreActorAdd(this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Projectile %s does not have an Instigator."), *GetNameSafe(this));
	}

	SphereComp->OnComponentEndOverlap.AddDynamic(this, &ASBlackholeProjectile::OnEndOverlap);

	// life timer
	GetWorldTimerManager().SetTimer(BlackholeLifeTimer, this, &ASBlackholeProjectile::EndBlackhole, 5.0f);
}

void ASBlackholeProjectile::EndBlackhole()
{
	this->Destroy();
}

void ASBlackholeProjectile::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->IsSimulatingPhysics())
	{
		OtherActor->Destroy();
	}
}

// Called every frame
void ASBlackholeProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RadialForceComp->FireImpulse();
}

