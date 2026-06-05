// Copyright Epic Games, Inc. All Rights Reserved.

#include "RuntimeIMDebugs.h"
#include "RuntimeIMDebugsWindow.h"

#define LOCTEXT_NAMESPACE "FRuntimeIMDebugsModule"

void FRuntimeIMDebugsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FRuntimeIMDebugsDockable::RegisterTab();
}

void FRuntimeIMDebugsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	FRuntimeIMDebugsDockable::UnregisterTab();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRuntimeIMDebugsModule, RuntimeIMDebugs)


