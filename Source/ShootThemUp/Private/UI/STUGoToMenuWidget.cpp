// Shoot Them Up Game, All Righs Reserved


#include "UI/STUGoToMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "STUGameInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGoToMenu, All, All);

void USTUGoToMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (MainMenuButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &USTUGoToMenuWidget::OnMainMenu);
	}
}

void USTUGoToMenuWidget::OnMainMenu()
{
	if (!GetWorld()) return;
	const auto STUGameInstance = GetWorld()->GetGameInstance<USTUGameInstance>();
	if (!STUGameInstance) return;
	if (STUGameInstance->GetMenuLevelName().IsNone())
	{
		UE_LOG(LogSTUGoToMenu, Error, TEXT("Menu level name is NONE"));
		return;
	}
	UGameplayStatics::OpenLevel(this, STUGameInstance->GetMenuLevelName());
}
