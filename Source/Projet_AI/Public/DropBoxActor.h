// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Subsystem/RecipeFactorySubsystem.h"
#include "DropBoxActor.generated.h"

UCLASS()
class PROJET_AI_API ADropBoxActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxCollision;
	UPROPERTY()
	TArray<FGameplayTag> RequiredTags;
	UPROPERTY()
	TArray<FGameplayTag> CurrentTags;
	UPROPERTY()
	TArray<ARecipeItem*> CurrentIngredients;

	UPROPERTY()
	FRecipeData RecipeData;

	bool bHasRecipe;

public:
	ADropBoxActor();

protected:
	virtual void BeginPlay() override;
	//This should be call when a recipe is completed
	UFUNCTION(BlueprintCallable)
	virtual void CompleteRecipe();

	//Use this for additional logic in blueprint 
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void CompleteRecipe_BP();

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void OnSetRecipe_BP(const FRecipeData InRecipeData);

public:
	// Fonction appelée lors d'un overlap
	UFUNCTION()
	void OnBeginOverlap(AActor* ThisActor, AActor* OtherActor);
	UFUNCTION()
	void OnEndOverlap(AActor* ThisActor, AActor* OtherActor);

	void SetRecipe(const FRecipeData& InRecipeData);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool HasRecipe() const { return bHasRecipe; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FRecipeData GetRecipe() const { return RecipeData; }
};
