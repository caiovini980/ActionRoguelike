// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "SAttributeComponent.h"
#include "SInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
	
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("BlackholeAttack", IE_Pressed, this, &ASCharacter::BlackholeAttack);
	PlayerInputComponent->BindAction("CastTeleport", IE_Pressed, this, &ASCharacter::CastTeleport);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0;
	ControlRot.Roll = 0;
	
	AddMovementInput(ControlRot.Vector(), Value);
}

void ASCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0;
	ControlRot.Roll = 0;

	// X = Forward
	// Y = Right
	// Z = Up
	
	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	AddMovementInput(RightVector, Value);
}

void ASCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}

void ASCharacter::ClassToSpawn(TSubclassOf<AActor>& ActorClass)
{
	if (!ensureAlways(ActorClass)) return;
	
	const float Distance = 10000.f;
	FVector TraceStart = CameraComp->GetComponentLocation();
	FVector FinalLocation = CameraComp->GetComponentLocation() + (CameraComp->GetForwardVector() * Distance);
	SpellSpawnLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	FCollisionShape Shape;
	Shape.SetSphere(20.0f);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	
	FHitResult Hit;
	bool bBlockingHit = GetWorld()->SweepSingleByObjectType(Hit, TraceStart, FinalLocation, FQuat::Identity, ObjectQueryParams, Shape, Params);

	// if it collides with something, use the collided position. If not, use the final position
	FVector SpawnFinalLocation = bBlockingHit ? Hit.Location : FinalLocation;

	FTransform SpawmTM = FTransform((SpawnFinalLocation - SpellSpawnLocation).Rotation(), SpellSpawnLocation);
	GetWorld()->SpawnActor<AActor>(ActorClass, SpawmTM, SpawnParams);
}

void ASCharacter::CastTeleport()
{
	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimerHandle_CastTeleport, this, &ASCharacter::CastTeleport_Timelapsed, TimeToAttack);
}

void ASCharacter::CastTeleport_Timelapsed()
{
	if(ensure(TeleportClass))
	{
		ClassToSpawn(TeleportClass);
	}
}

void ASCharacter::BlackholeAttack()
{
	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimerHandle_BlackholeAttack, this, &ASCharacter::BlackholeAttack_Timelapsed, TimeToAttack);
}

void ASCharacter::BlackholeAttack_Timelapsed()
{
	if (ensure(BlackholeClass))
	{
		ClassToSpawn(BlackholeClass);
	}
}

void ASCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeLapsed, TimeToAttack);
}

void ASCharacter::PrimaryAttack_TimeLapsed()
{
	if (ensure(ProjectileClass))
	{
		ClassToSpawn(ProjectileClass);
		
		if (ensure(PrimaryAttackEffectOnOrigin))
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PrimaryAttackEffectOnOrigin, SpellSpawnLocation);
		}
	}
}


void ASCharacter::OnHealthChanged(AActor* IntigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		DisableInput(PlayerController);
	}
}

