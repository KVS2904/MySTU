// Shoot Them Up Game, All Righs Reserved


#include "Components/STUHealthComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraShakeBase.h"
#include "STUGameModeBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All)

USTUHealthComponent::USTUHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USTUHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	check(MaxHealth > 0);

	Health = MaxHealth;
	//OnHealthChanged.Broadcast(Health);

	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
	}
}

void USTUHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f || IsDead()) return;

	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(Health);

	if (GetWorld())
	{
		LastDamageTime = GetWorld()->GetTimeSeconds();
	}
	

	if (IsDead())
	{
		Killed(InstigatedBy);
		OnDeath.Broadcast();
	}

	PlayCameraShake();
}

void USTUHealthComponent::AutoHeal()
{
	if (bAutoHeal)
	{
		if (Health < MaxHealth)
		{
			float CurrentTime = (GetWorld()->GetTimeSeconds());
			//UE_LOG(LogTemp, Display, TEXT("World time %f"), CurrentTime)
			//UE_LOG(LogTemp, Display, TEXT("Last damage time %f"), LastDamageTime)
			if ((CurrentTime - LastDamageTime) > HealDelay)
			{
				if ((CurrentTime - LastAutoHealTime) > HealUpdateTime)
				{
					//UE_LOG(LogTemp, Display, TEXT("It is auto heal!"));
					Health = FMath::Clamp(Health + HealModifier, 0.0f, MaxHealth);
					//OnHealthChanged.Broadcast(Health);
					LastAutoHealTime = CurrentTime;
				}
				
			}
		}
	}
}

bool USTUHealthComponent::TryToAddHealth(float HealthAmount)
{
	if (Health >= MaxHealth || HealthAmount <= 0.0f) return false;
	Health = FMath::Clamp(Health + HealthAmount, 0.0f, MaxHealth);
	//OnHealthChanged.Broadcast(Health);
	return true;
}

void USTUHealthComponent::PlayCameraShake()
{
	if (IsDead()) return;

	const auto Player = Cast<APawn>(GetOwner());
	if (!Player) return;

	const auto Controller = Player->GetController<APlayerController>();
	if (!Controller || !Controller->PlayerCameraManager) return;

	Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}

void USTUHealthComponent::Killed(AController* KillerController)
{
	if (!GetWorld())return;
	const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
	if (!GameMode) return;

	const auto Player = Cast<APawn>(GetOwner());
	const auto VictimController = Player ? Player->Controller : nullptr;

	GameMode->Killed(KillerController, VictimController);
}
