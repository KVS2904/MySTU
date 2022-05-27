// Shoot Them Up Game, All Righs Reserved

#pragma once

#include "CoreMinimal.h"
#include "Player/STUBaseCharacter.h"
#include "STUAICharacter.generated.h"

class UBehaviorTree;
class UWidgetComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUAICharacter : public ASTUBaseCharacter
{
	GENERATED_BODY()
	
public:
	ASTUAICharacter(const FObjectInitializer& ObjInit);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		UBehaviorTree* BehaviorTreeAsset;

	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UWidgetComponent* HealthWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		float HealthVisibilityDistance = 1000.0f;

	virtual void BeginPlay() override;
	virtual void OnDeath() override;
	virtual void OnHealthChanged(float Health) override;

private:
	void UpdateHealthWidgetVisibility();
};

