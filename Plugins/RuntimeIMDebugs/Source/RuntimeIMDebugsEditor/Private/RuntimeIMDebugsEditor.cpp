// Copyright Epic Games, Inc. All Rights Reserved.

#include "RuntimeIMDebugsEditor.h"
#include "RuntimeIMDebugsWindow.h"


void FRuntimeIMDebugsModuleEditor::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FRuntimeIMDebugsDockable::RegisterTab();

	WorldInitializationHandle = FWorldDelegates::OnWorldInitializedActors.AddRaw(this,	&FRuntimeIMDebugsModuleEditor::OnPostWorldInitialization);
	StartPIEHandle = FEditorDelegates::StartPIE.AddRaw(this, &FRuntimeIMDebugsModuleEditor::OnStartPIE);
}

void FRuntimeIMDebugsModuleEditor::ShutdownModule()
{

	if(WorldInitializationHandle.IsValid())
		FWorldDelegates::OnPostWorldInitialization.Remove(WorldInitializationHandle);
	
	if (StartPIEHandle.IsValid())
		FEditorDelegates::StartPIE.Remove(StartPIEHandle);


	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FRuntimeIMDebugsDockable::UnregisterTab();

}

void FRuntimeIMDebugsModuleEditor::OnPostWorldInitialization(const FActorsInitializedParams& InitializationParams)
{
	UE_LOG(LogTemp, Warning, TEXT("On post world initialization called re-make the full widget to avoid dangling tabs"));

	FRuntimeIMDebugsDockable::RecreateWidget();
	
}

void FRuntimeIMDebugsModuleEditor::OnStartPIE(const bool InIsSimulating)
{
	FRuntimeIMDebugsDockable::OnStartPIE();

}
	
IMPLEMENT_MODULE(FRuntimeIMDebugsModuleEditor, RuntimeIMDebugsEditor)


