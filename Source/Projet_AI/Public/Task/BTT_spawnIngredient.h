// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ingredient.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_spawnIngredient.generated.h"

/**
 * 
 */
UCLASS()
class PROJET_AI_API UBTT_spawnIngredient : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AIngredient> SpawnedIngredient;

private:

	FVector SpawnLocation;

	//class UNavigationSystemV1* NavArea;
};
