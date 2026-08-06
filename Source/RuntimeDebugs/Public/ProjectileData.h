// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ProjectileData.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FProjectileInfo 
{
	GENERATED_BODY()

	FProjectileInfo() : Damage(5), LifeTime(5), HomingProjectile(false), HomingAcceleration(3000), MaxSpeed(500), GravityScale(0)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileInfo")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileInfo")
	float LifeTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileInfo")
	bool HomingProjectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileInfo")
	float HomingAcceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileInfo")
	float MaxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileInfo")
	float GravityScale;
};

UCLASS(Blueprintable)
class RUNTIMEDEBUGS_API UProjectileData : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileData")
	FProjectileInfo Data;
	
};
