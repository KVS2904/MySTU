// Shoot Them Up Game, All Righs Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUGoToMenuWidget.generated.h"

class UButton;
UCLASS()
class SHOOTTHEMUP_API USTUGoToMenuWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UButton* MainMenuButton;

protected:
	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
		void OnMainMenu();
	
};
