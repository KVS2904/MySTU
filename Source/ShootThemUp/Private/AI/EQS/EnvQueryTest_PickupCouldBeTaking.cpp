// Shoot Them Up Game, All Righs Reserved


#include "AI/EQS/EnvQueryTest_PickupCouldBeTaking.h"
#include "Pickups/STUBasePickup.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"

UEnvQueryTest_PickupCouldBeTaking::UEnvQueryTest_PickupCouldBeTaking(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Cost = EEnvTestCost::Low;
	ValidItemType = UEnvQueryItemType_ActorBase::StaticClass();
	SetWorkOnFloatValues(false);
}

void UEnvQueryTest_PickupCouldBeTaking::RunTest(FEnvQueryInstance& QueryInstance) const
{
	const auto DataOwner = QueryInstance.Owner.Get();
	BoolValue.BindData(DataOwner, QueryInstance.QueryID);
	const bool bWantsBeTakable = BoolValue.GetValue();

	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		const auto ItemActor = GetItemActor(QueryInstance, It.GetIndex());
		const auto PickupActor = Cast<ASTUBasePickup>(ItemActor);
		if (!PickupActor) continue;

		const auto CouldBeTaken = PickupActor->CouldBeTaken();
		It.SetScore(TestPurpose, FilterType, CouldBeTaken, bWantsBeTakable);
	}
}