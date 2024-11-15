// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJET_AI_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()
	
public:

    AAIControllerBase(const FObjectInitializer& ObjectInitializer);

    virtual void OnPossess(APawn* InPawn) override;

    UPROPERTY(Transient)
    class UBehaviorTreeComponent* BehaviorTreeComponent;

    // BlackBoard
    UPROPERTY(Transient)
    class UBlackboardComponent* BlackboardComponent;
};