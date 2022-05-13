// Shoot Them Up Game, All Righs Reserved


#include "Player/STUPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/STUWeaponComponent.h"
#include "Components/STUCharacterMovementComponent.h"


ASTUPlayerCharacter::ASTUPlayerCharacter(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SocketOffset = FVector(0.0f, 100.0f, 80.0f);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void ASTUPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	check(WeaponComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASTUPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASTUPlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ASTUPlayerCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnAround", this, &ASTUPlayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUPlayerCharacter::Jump);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASTUPlayerCharacter::OnStartRunning);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ASTUPlayerCharacter::OnStopRunning);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &USTUWeaponComponent::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &USTUWeaponComponent::StopFire);
	PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &USTUWeaponComponent::NextWeapon);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &USTUWeaponComponent::Reload);
}

bool ASTUPlayerCharacter::IsRunning() const
{
	return bIsRunning;
}

void ASTUPlayerCharacter::MoveForward(float Amount)
{
	if (Amount == 0.0f)
	{
		bIsMovingForward = false;
		return;
	}
	bIsMovingForward = Amount > 0.0f;
	bIsRunning = bIsMovingForward && bWantsRunning && bMaxSpeedIncreased && !GetVelocity().IsZero();
	//UE_LOG(LogSTUBaseCharacter, Display, TEXT("Is running: %d"), IsRunning());
	AddMovementInput(GetActorForwardVector(), Amount);
}

void ASTUPlayerCharacter::MoveRight(float Amount)
{
	if (Amount == 0.0f) return;
	AddMovementInput(GetActorRightVector(), Amount);
}

void ASTUPlayerCharacter::OnStartRunning()
{
	bWantsRunning = true;
	if (bIsMovingForward)
	{
		if (FMath::Abs(GetMovementDirection()) < 10)
		{
			bMaxSpeedIncreased = true;
			CurrentMovementComponent->SetMaxSpeedForRun(bWantsRunning);
		}
	}

}

void ASTUPlayerCharacter::OnStopRunning()
{
	bWantsRunning = false;
	if (bMaxSpeedIncreased == true)
	{
		CurrentMovementComponent->SetMaxSpeedForRun(bWantsRunning);
		bMaxSpeedIncreased = false;
	}
}

void ASTUPlayerCharacter::OnDeath()
{
	Super::OnDeath();
	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}
}
