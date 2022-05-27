// Shoot Them Up Game, All Righs Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUCoreTypes.h"
#include "STUHealthComponent.generated.h"

class UCameraShakeBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTUHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USTUHealthComponent();

	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintCallable, Category = "Health")
	bool IsDead() const { return Health <= 0; }

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealthPercent() const { return Health / MaxHealth; }

	void AutoHeal();

	bool TryToAddHealth(float HealthAmount);

	FOnDeath OnDeath;
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
		bool bAutoHeal = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0.0", ClampMax = "15.0"))
		float HealUpdateTime = 0.1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0.0", ClampMax = "100.0"))
		float HealDelay = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0.0", ClampMax = "50.0"))
		float HealModifier = 1;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
		float MaxHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
		TSubclassOf<UCameraShakeBase> CameraShake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
		TMap<UPhysicalMaterial*, float> DamageModifiers;

	virtual void BeginPlay() override;

private:
	float Health = 0.0f;
	float LastDamageTime = 0.0f;
	float LastAutoHealTime = 0.0f;

	UFUNCTION()
		void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	
	UFUNCTION()
		void OnTakePointDamage(AActor* DamagedActor, float Damage,
			class AController* InstigatedBy, FVector HitLocation,
			class UPrimitiveComponent* FHitComponent, FName BoneName,
			FVector ShotFromDirection, const class UDamageType* DamageType, 
			AActor* DamageCauser);

	UFUNCTION()
		void OnTakeRadialDamage(AActor* DamagedActor, float Damage, 
			const class UDamageType* DamageType, FVector Origin, 
			FHitResult HitInfo, class AController* InstigatedBy,
			AActor* DamageCauser);

	void ApplyDamage(float Damage, AController* InstigatedBy);
	float GetPointDamageModifier(AActor* DamagedActor, const FName& BoneName);

	void PlayCameraShake();
	void Killed(AController* KillerController);

	void ReportDamageEvent(float Damage, AController* InstigatedBy);
};
