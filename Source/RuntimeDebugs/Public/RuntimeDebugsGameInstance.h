// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RuntimeDebugsGameInstance.generated.h"

/**
 * 
 */

class UProjectileData;

UCLASS()
class RUNTIMEDEBUGS_API URuntimeDebugsGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	void Init();

	void Shutdown();

	void OnWorldInitialized(const FActorsInitializedParams&);

	UFUNCTION(BlueprintImplementableEvent)
	void OnWorldInitialized();

protected:
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Debug")
	UProjectileData* GetProjectileDataByName(TArray<UProjectileData*> InProjectiles, FName InProjectileName) const;

	UFUNCTION(Category = "Debug")
	void OnDebugToggleChanged(const FName InTabID, const FName InSectionID, const FName InID, bool InValue);

	UFUNCTION(Category = "Debug")
	void OnDebugSpinBoxChanged(const FName InTabID, const FName InSectionID, const FName InID, float InValue);

	UFUNCTION(Category = "Debug")
	void OnDebugComboBoxChanged(const FName InTabID, const FName InSectionID, const FName InID, int InIndex);

	UFUNCTION(Category = "Debug")
	void OnDebugButtonPressed(const FName InTabID, const FName InSectionID, const FName InID);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Debugs")
	TArray<FString> LevelNames;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Debugs")
	int DebugLoadLevelIndex;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Debugs")
	TArray<TObjectPtr<UProjectileData>> DebugProjectilesData;

private:

	FDelegateHandle PostLoadMapWithWorldHandle;

};
