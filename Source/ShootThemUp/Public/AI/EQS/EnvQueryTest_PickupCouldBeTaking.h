// Shoot Them Up Game, All Righs Reserved

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_PickupCouldBeTaking.generated.h"

UCLASS()
class SHOOTTHEMUP_API UEnvQueryTest_PickupCouldBeTaking : public UEnvQueryTest
{
	GENERATED_BODY()
	
public:
	UEnvQueryTest_PickupCouldBeTaking(const FObjectInitializer& ObjectInitializer);
	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;
};
