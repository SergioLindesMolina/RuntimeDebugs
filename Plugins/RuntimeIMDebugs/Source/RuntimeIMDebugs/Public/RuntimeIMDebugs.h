// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

/**
 * RuntimeIMDebugs
 *
 * Runtime immediate mode debugging framework for Unreal Engine.
 *
 * This plugin provides a lightweight runtime debug window designed to quickly
 * expose gameplay and system values during PIE or runtime execution.
 *
 * The framework allows programmers and designers to dynamically create:
 *		- Buttons
 *		- Toggles
 *		- Sliders
 *		- Float fields
 *
 * organized into debug tabs using a simple subsystem-driven API.
 *
 * RuntimeIMDebugs is designed for fast iteration and temporary gameplay
 * instrumentation without requiring custom editor tooling or complex UI setup.
 *
 * The system is built on top of SlateIM
 *
 * Typical use cases include:
 *		- Weapon tuning
 *		- AI parameter tweaking
  *		- Debug commands
 *		- Gameplay testing utilities
 *		- Experimental feature controls
 */


class FRuntimeIMDebugsModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

};
