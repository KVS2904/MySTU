// Shoot Them Up Game, All Righs Reserved


#include "Components/STUCharacterMovementComponent.h"

void USTUCharacterMovementComponent::SetMaxSpeedForRun(bool bWantsRunning)
{
	if (bWantsRunning)
	{ 
		MaxWalkSpeed = GetMaxSpeed() * RunModifier;
	}
	else
	{
		MaxWalkSpeed = GetMaxSpeed() / RunModifier;
	}

	UE_LOG(LogTemp, Display, TEXT("Max Walk Speed: %f"), MaxWalkSpeed);
	
}
