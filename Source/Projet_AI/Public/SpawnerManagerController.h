// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SpawnerManagerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJET_AI_API ASpawnerManagerController : public AAIController
{
	GENERATED_BODY()

public:

	ASpawnerManagerController(const FObjectInitializer& ObjectInitializer);

	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(Transient)
	class UBehaviorTreeComponent* BehaviorTreeComponent;
};
