#pragma once

#include "CoreMinimal.h"
#include"GameplayTagContainer.h"
#include "IngredientTable.generated.h"

UENUM(BlueprintType,Blueprintable)
enum class EIngredientType : uint8 { Meet, Fruit_Vegetable, Dairy, Cereal };

USTRUCT()
struct FIngredientTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EIngredientType Type;
	
};
