#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RecipeItem.generated.h"

UCLASS()
class COOKINGFACTORY_API ARecipeItem : public AActor
{
	GENERATED_BODY()
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* SphereCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMesh;

public:
	ARecipeItem();

	FORCEINLINE void SetMesh(UStaticMesh* NewMesh) 
	{
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
