// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "AIControllerBase.generated.h"


/**
 * 
 */
UCLASS()
class PROJET_AI_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
    AAIControllerBase(const FObjectInitializer& ObjectInitializer);

    virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
    UFUNCTION()
	void OnTargetPerceptionForgotten(AActor* Actor);

	UFUNCTION(BlueprintCallable)
	void BackToNavMesh();

	class AAICharacter* possessedAi;

    UPROPERTY(Transient)
    class UBehaviorTreeComponent* BehaviorTreeComponent;

    // BlackBoard
    UPROPERTY(Transient)
    class UBlackboardComponent* BlackboardComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    TSubclassOf<AActor> IngredientClass;
	
};