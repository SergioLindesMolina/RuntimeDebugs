// Fill out your copyright notice in the Description page of Project Settings.


#include "RuntimeDebugsGameInstance.h"


void URuntimeDebugsGameInstance::Init() 
{
	Super::Init();

	PostLoadMapWithWorldHandle = FWorldDelegates::OnWorldInitializedActors.AddUObject(this, &URuntimeDebugsGameInstance::OnWorldInitialized);

}

void URuntimeDebugsGameInstance::Shutdown() 
{
	if (PostLoadMapWithWorldHandle.IsValid()) 
	{
		FWorldDelegates::OnWorldInitializedActors.Remove(PostLoadMapWithWorldHandle);
	}

	Super::Shutdown();
}


void URuntimeDebugsGameInstance::OnWorldInitialized(const FActorsInitializedParams&)
{
	OnWorldInitialized();
}
