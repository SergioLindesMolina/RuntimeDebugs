// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FRuntimeIMDebugsExposed;


class FRuntimeIMDebugsModuleEditor : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;


private:

	void OnPostWorldInitialization(const FActorsInitializedParams& InitializationParams);
	void OnStartPIE(const bool InIsSimulating);

	FDelegateHandle WorldInitializationHandle;
	FDelegateHandle StartPIEHandle;
};
