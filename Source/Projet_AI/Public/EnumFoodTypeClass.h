#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnumFoodTypeClass.generated.h"

UENUM(BlueprintType, Category = "TypeAlimentaire")
enum class EnumFoodTypeClass : uint8
{
	Cereal UMETA(DisplayName = "Cereal"),
	FruitAndVegetable UMETA(DisplayName = "Fruit and Vegetable"),
	Dairy UMETA(DisplayName = "Dairy"),
	MeatAndSubstitute UMETA(DisplayName = "Meat and Substitute")
};

/*
switch (FoodCategory)
{
	case EFoodCategory::Cereal:
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Cereal category"));
		break;
	case EFoodCategory::FruitAndVegetable:
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Fruit and Vegetable category"));
		break;
	case EFoodCategory::Dairy:
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Dairy category"));
		break;
	case EFoodCategory::MeatAndSubstitute:
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Meat and Substitute category"));
		break;
	default:
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Unknown food category"));
		break;
}
*/
