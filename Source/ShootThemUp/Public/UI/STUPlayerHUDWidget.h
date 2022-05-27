// Shoot Them Up Game, All Righs Reserved

#pragma once

#include "CoreMinimal.h"
#include "UI/STUBaseWidget.h"
#include "STUCoreTypes.h"
#include "STUPlayerHUDWidget.generated.h"

class UProgressBar;
class UWidgetAnimation;

UCLASS()
class SHOOTTHEMUP_API USTUPlayerHUDWidget : public USTUBaseWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "UI")
		float GetHealthPercent() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
		bool GetWeaponUIData(FWeaponUIData& UIData) const;

	UFUNCTION(BlueprintCallable, Category = "UI")
		bool GetWeaponAmmoData(FAmmoData& CurrentAmmo, FAmmoData& DefaultAmmo) const;

	UFUNCTION(BlueprintCallable, Category = "UI")
		bool IsPlayerAlive() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
		bool IsPlayerSpectating() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
		void OnTakeDamage();

	UFUNCTION(BlueprintCallable, Category = "UI")
		int32 GetKillsNum() const;

	virtual bool Initialize() override;

protected:
	UPROPERTY(meta = (BindWidget))
		UProgressBar* HealthProgressBar;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* DamageAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		float PercentColorThreshold = 0.3f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		FLinearColor GoodColor = FLinearColor::White;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		FLinearColor BadColor = FLinearColor::Red;

private:
	float LastHealth = 0.0f;

	void OnHealthChanged(float Health);
	void OnNewPawn(APawn* NewPawn);
	void UpdateHealthBar();
};
