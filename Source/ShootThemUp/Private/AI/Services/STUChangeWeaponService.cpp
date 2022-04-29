// Shoot Them Up Game, All Righs Reserved


#include "AI/Services/STUChangeWeaponService.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUWeaponComponent.h"

USTUChangeWeaponService::USTUChangeWeaponService()
{
	NodeName = "Change Weapon";
}

void USTUChangeWeaponService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (FMath::RandRange(0.0f, 1.0f) <= Probabilty && Probabilty > 0.0f)
	{
		const auto Controller = OwnerComp.GetAIOwner();
		if (Controller)
		{
			const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Controller->GetPawn());
			if (WeaponComponent)
			{
				WeaponComponent->NextWeapon();
			}
		}
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}