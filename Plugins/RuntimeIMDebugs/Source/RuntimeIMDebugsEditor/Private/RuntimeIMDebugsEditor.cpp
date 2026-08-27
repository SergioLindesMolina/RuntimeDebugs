// Copyright Epic Games, Inc. All Rights Reserved.

#include "RuntimeIMDebugsEditor.h"
#include "RuntimeIMDebugsWindow.h"


void FRuntimeIMDebugsModuleEditor::StartupModule()
{
	FRuntimeIMDebugsDockable::RegisterTab();

	StartPIEHandle = FEditorDelegates::StartPIE.AddRaw(this, &FRuntimeIMDebugsModuleEditor::OnStartPIE);
}

void FRuntimeIMDebugsModuleEditor::ShutdownModule()
{

	if (StartPIEHandle.IsValid())
		FEditorDelegates::StartPIE.Remove(StartPIEHandle);

	FRuntimeIMDebugsDockable::UnregisterTab();

}


void FRuntimeIMDebugsModuleEditor::OnStartPIE(const bool InIsSimulating)
{
	FRuntimeIMDebugsDockable::OnStartPIE();

}
	
IMPLEMENT_MODULE(FRuntimeIMDebugsModuleEditor, RuntimeIMDebugsEditor)


