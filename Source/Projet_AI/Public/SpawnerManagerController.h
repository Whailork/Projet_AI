// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameplayTagContainer.h"
#include "Subsystem/RecipeFactorySubsystem.h"
#include "SpawnerManagerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJET_AI_API ASpawnerManagerController : public AAIController
{
	GENERATED_BODY()
	
	FVector SpawnLocation;
	TArray<FGameplayTag> toSpawnList;

public:

	ASpawnerManagerController(const FObjectInitializer& ObjectInitializer);

	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(Transient)
	class UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(Transient)
	class UBlackboardComponent* BlackboardComponent;

	UFUNCTION()
	void OnNewRecipe(FRecipeData data);
	UFUNCTION(BlueprintCallable)
	bool GetNextIngredient(FGameplayTag& outTag);

};
