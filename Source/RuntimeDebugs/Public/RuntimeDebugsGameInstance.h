// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RuntimeDebugsGameInstance.generated.h"

/**
 * 
 */
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


private:

	FDelegateHandle PostLoadMapWithWorldHandle;

};
