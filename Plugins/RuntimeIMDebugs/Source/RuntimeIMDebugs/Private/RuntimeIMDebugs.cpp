// Copyright Epic Games, Inc. All Rights Reserved.

#include "RuntimeIMDebugs.h"
#include "RuntimeIMDebugsSubsystem.h"
#include "RuntimeIMDebugsLog.h"

#include "Engine/Engine.h"
#include "HAL/IConsoleManager.h"

#define LOCTEXT_NAMESPACE "FRuntimeIMDebugsModule"

void FRuntimeIMDebugsModule::StartupModule()
{
	//Register Console Command
	if (!IConsoleManager::Get().IsNameRegistered(TEXT("RuntimeIMDebugs.ShowWindow")))
	{
		ShowWindowConsoleCommand = IConsoleManager::Get().RegisterConsoleCommand(
			TEXT("RuntimeIMDebugs.ShowWindow"),
			TEXT("Shows or hides the RuntimeIMDebugs window. 0 = Hide, 1 = Show. NoArgument = Toggle"),
			FConsoleCommandWithArgsDelegate::CreateRaw(
				this,
				&FRuntimeIMDebugsModule::HandleShowWindowConsoleCommand),
			ECVF_Default
		);
	}
	else
	{
		UE_LOG(LogRuntimeIMDebugs, Error, TEXT("Error Registering the Command : Already exist a comand registerd with the name RuntimeIMDebugs.ShowWindow"));
	}

}

void FRuntimeIMDebugsModule::ShutdownModule()
{
	if (ShowWindowConsoleCommand)
	{
		IConsoleManager::Get().UnregisterConsoleObject(ShowWindowConsoleCommand);
		ShowWindowConsoleCommand = nullptr;
	}
}

void FRuntimeIMDebugsModule::HandleShowWindowConsoleCommand(const TArray<FString>& Args)
{

	if (!GEngine)
	{
		return;
	}

	UWorld* CurrentWorld = GEngine->GetCurrentPlayWorld();

	if (!CurrentWorld)
	{
		return;
	}

	URuntimeIMDebugsSubsystem* RuntimeIMDebugsSubsystem = CurrentWorld->GetSubsystem<URuntimeIMDebugsSubsystem>();

	if (!RuntimeIMDebugsSubsystem)
	{
		return;
	}


	if (Args.IsEmpty())
	{
		RuntimeIMDebugsSubsystem->ToggleWindow();
		return;
	}

	const bool bShowWindow = Args[0] == "1";
	const bool bHideWindow = Args[0] == "0";

	if (bShowWindow)
	{
		RuntimeIMDebugsSubsystem->ShowWindow();
	}
	else if (bHideWindow)
	{
		RuntimeIMDebugsSubsystem->HideWindow();
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRuntimeIMDebugsModule, RuntimeIMDebugs)


