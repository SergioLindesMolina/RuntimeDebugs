// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyData.generated.h"

/**
 * 
 */

class ABaseProjectile;

USTRUCT(BlueprintType)
struct FEnemyInfo 
{
	GENERATED_BODY()

	FEnemyInfo() : AttackCooldown(10), AttackRange(400), MaxHealth(100)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyInfo")
	float AttackCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyInfo")
	float AttackRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyInfo")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyInfo")
	TSubclassOf<ABaseProjectile> Projectile;

};

UCLASS(Blueprintable)
class RUNTIMEDEBUGS_API UEnemyData : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyData")
	FEnemyInfo Data;
	
};
