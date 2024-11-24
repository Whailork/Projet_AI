// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayTags.h"
#include "GameFramework/Pawn.h"
#include "SpawnManagerPawn.generated.h"

UCLASS()
class PROJET_AI_API ASpawnManagerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASpawnManagerPawn();
	
	UPROPERTY(EditAnywhere, Category = "AI")
	class UBehaviorTree* TreeAsset;

	//UPROPERTY(EditAnywhere, Category = "Tag")
	//FGameplayTag SpawnTag;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
