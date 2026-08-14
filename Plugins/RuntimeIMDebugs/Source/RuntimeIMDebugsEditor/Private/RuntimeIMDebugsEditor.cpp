// Copyright Epic Games, Inc. All Rights Reserved.

#include "RuntimeIMDebugsEditor.h"
#include "RuntimeIMDebugsWindow.h"


void FRuntimeIMDebugsModuleEditor::StartupModule()
{

	FRuntimeIMDebugsDockable::RegisterTab();

	WorldInitializationHandle = FWorldDelegates::OnWorldInitializedActors.AddRaw(this,	&FRuntimeIMDebugsModuleEditor::OnPostWorldInitialization);
	StartPIEHandle = FEditorDelegates::StartPIE.AddRaw(this, &FRuntimeIMDebugsModuleEditor::OnStartPIE);
}

void FRuntimeIMDebugsModuleEditor::ShutdownModule()
{

	if(WorldInitializationHandle.IsValid())
		FWorldDelegates::OnWorldInitializedActors.Remove(WorldInitializationHandle);
	
	if (StartPIEHandle.IsValid())
		FEditorDelegates::StartPIE.Remove(StartPIEHandle);

	FRuntimeIMDebugsDockable::UnregisterTab();

}

void FRuntimeIMDebugsModuleEditor::OnPostWorldInitialization(const FActorsInitializedParams& InitializationParams)
{
	// It is necessary to recreate the widget when changing maps; otherwise, some tabs may be left dangling.
	FRuntimeIMDebugsDockable::RecreateWidget();
	
}

void FRuntimeIMDebugsModuleEditor::OnStartPIE(const bool InIsSimulating)
{
	FRuntimeIMDebugsDockable::OnStartPIE();

}
	
IMPLEMENT_MODULE(FRuntimeIMDebugsModuleEditor, RuntimeIMDebugsEditor)


