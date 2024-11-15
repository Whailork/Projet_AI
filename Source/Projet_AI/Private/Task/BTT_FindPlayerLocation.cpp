// Fill out your copyright notice in the Description page of Project Settings.


#include "Task/BTT_FindPlayerLocation.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTT_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // Get the navigation Area in the current level
    NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

    if (NavArea)
    {
        // Get the current player position
        FVector PlayerPosition = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

        // Return true if found a random point in the navigation and around your player position
        if (NavArea->K2_GetRandomPointInNavigableRadius(GetWorld(), PlayerPosition, TargetLocation, 100.0f))
        {
            // Set value in the blackboard
            OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName("TargetLocation"), TargetLocation);

            return EBTNodeResult::Succeeded;
        }
    }
    return EBTNodeResult::Failed;
}