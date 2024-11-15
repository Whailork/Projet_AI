// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_FindPlayerLocation.generated.h"

/**
 * 
 */
UCLASS()
class PROJET_AI_API UBTT_FindPlayerLocation : public UBTTaskNode
{
	GENERATED_BODY()
	
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:

    FVector TargetLocation;

    class UNavigationSystemV1* NavArea;
};