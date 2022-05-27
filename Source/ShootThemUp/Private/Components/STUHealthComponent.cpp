// Shoot Them Up Game, All Righs Reserved


#include "Components/STUHealthComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraShakeBase.h"
#include "STUGameModeBase.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Perception/AISense_Damage.h"

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
	OnHealthChanged.Broadcast(Health);

	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
		ComponentOwner->OnTakePointDamage.AddDynamic(this, &USTUHealthComponent::OnTakePointDamage);
		ComponentOwner->OnTakeRadialDamage.AddDynamic(this, &USTUHealthComponent::OnTakeRadialDamage);
	}
}

void USTUHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, 
	const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{

}

void USTUHealthComponent::OnTakePointDamage(AActor* DamagedActor, float Damage,
	class AController* InstigatedBy, FVector HitLocation,
	class UPrimitiveComponent* FHitComponent, FName BoneName,
	FVector ShotFromDirection, const class UDamageType* DamageType,
	AActor* DamageCauser)
{
	const auto FinalDamage = Damage * GetPointDamageModifier(DamagedActor, BoneName);
	ApplyDamage(FinalDamage, InstigatedBy);
}

void USTUHealthComponent::OnTakeRadialDamage(AActor* DamagedActor, float Damage,
	const class UDamageType* DamageType, FVector Origin,
	FHitResult HitInfo, class AController* InstigatedBy,
	AActor* DamageCauser)
{
	ApplyDamage(Damage, InstigatedBy);
}

void USTUHealthComponent::ApplyDamage(float Damage, AController* InstigatedBy)
{
	if (Damage <= 0.0f || IsDead()) return;

	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(Health);

	if (GetWorld())	LastDamageTime = GetWorld()->GetTimeSeconds();

	if (IsDead())
	{
		Killed(InstigatedBy);
		OnDeath.Broadcast();
	}

	PlayCameraShake();
	ReportDamageEvent(Damage, InstigatedBy);
}

float USTUHealthComponent::GetPointDamageModifier(AActor* DamagedActor, const FName& BoneName)
{
	const auto Character = Cast<ACharacter>(DamagedActor);
	if (!Character ||
		!Character->GetMesh() ||
		!Character->GetMesh()->GetBodyInstance(BoneName)) return 1.0f;

	const auto PhysMaterial = Character->GetMesh()->GetBodyInstance(BoneName)->GetSimplePhysicalMaterial();
	if (!PhysMaterial || !DamageModifiers.Contains(PhysMaterial)) return 1.0f;

	return DamageModifiers[PhysMaterial];
}

void USTUHealthComponent::AutoHeal()
{
	if (bAutoHeal)
	{
		if (Health < MaxHealth)
		{
			float CurrentTime = (GetWorld()->GetTimeSeconds());
			if ((CurrentTime - LastDamageTime) > HealDelay)
			{
				if ((CurrentTime - LastAutoHealTime) > HealUpdateTime)
				{
					Health = FMath::Clamp(Health + HealModifier, 0.0f, MaxHealth);
					OnHealthChanged.Broadcast(Health);
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
	OnHealthChanged.Broadcast(Health);
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

void USTUHealthComponent::ReportDamageEvent(float Damage, AController* InstigatedBy)
{
	if (!InstigatedBy || !InstigatedBy->GetPawn() || !GetOwner()) return;

	UAISense_Damage::ReportDamageEvent(GetWorld(), 
		GetOwner(), 
		InstigatedBy->GetPawn(), 
		Damage, 
		InstigatedBy->GetPawn()->GetActorLocation(), 
		GetOwner()->GetActorLocation());
}
