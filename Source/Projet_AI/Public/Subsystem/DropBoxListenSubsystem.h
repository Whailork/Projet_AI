// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DropBoxActor.h"
#include "Subsystems/WorldSubsystem.h"
#include "DropBoxListenSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class PROJET_AI_API UDropBoxListenSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	void SubscribeDropBox(ADropBoxActor* DropBox);
	void UnSubscribeDropBox(ADropBoxActor* DropBox);

protected:
	UFUNCTION()
	void SetNewRecipe(const FRecipeData RecipeData);
private:
	UPROPERTY()
	TArray<ADropBoxActor*>ActiveDropBox;
};
