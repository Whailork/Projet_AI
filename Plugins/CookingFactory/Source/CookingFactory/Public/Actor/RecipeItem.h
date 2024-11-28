#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include"GameplayTagContainer.h"
#include "RecipeItem.generated.h"

enum class EIngredientType : uint8;

UCLASS()
class COOKINGFACTORY_API ARecipeItem : public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* SphereCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionStimuliSourceComponent* StimuliSource;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag IngredientTag;

	UPROPERTY(BlueprintReadOnly)
	EIngredientType IngredientType;

	UPROPERTY()
	bool InDroppingZone;
	UFUNCTION(BlueprintCallable)
	void SetInDroppingZone(bool value);
	UFUNCTION(BlueprintCallable)
	bool IsInDroppingZone();
public:
	ARecipeItem();

	FORCEINLINE void SetMesh(UStaticMesh* NewMesh) 
	{
		if(NewMesh ==nullptr)return;
		
		if(!HasActorBegunPlay())
		{
			IngredientMesh = NewMesh;
		}
		else
		{
			StaticMesh->SetStaticMesh(NewMesh);

		}
	}

protected:
	virtual void BeginPlay() override;


private:
	UPROPERTY()
	UStaticMesh* IngredientMesh;
};
