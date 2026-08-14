# RuntimeIMDebugs

## Contents

- [About](#about)
- [Features](#features)
- [Requirements](#requirements)
- [Installation](#installation)
  - [Clone from GitHub](#clone-from-github)
  - [Setup](#setup)
- [Configuration](#configuration)
- [Architecture](#architecture)
- [API](#api)
- [Console Command](#console-command)
- [Supported Debug Fields](#supported-debug-fields)
- [Extending RuntimeIMDebugs](#extending-runtimeimdebugs)
- [Project Structure](#project-structure)
- [License](#license)

## About

RuntimeIMDebugs is a free and open-source Unreal Engine plugin that provides a lightweight runtime debug window for exposing gameplay and system information during development.

The plugin allows programmers and designers to create runtime debug interfaces directly from C++ without having to create custom UMG widgets or editor tools.

RuntimeIMDebugs is built on top of SlateIM and is designed for fast iteration, gameplay debugging, parameter tuning, and temporary runtime instrumentation.

The plugin is designed primarily for use during PIE sessions and is not currently intended for Standalone or Shipping builds. It may be possible to extend RuntimeIMDebugs to support these modes, and support for them may be added in the future.

The main goal of RuntimeIMDebugs is to provide a simple way to create a small debug window for testing and adjusting gameplay systems that are difficult to handle with simple log messages or `PrintString`. It can be used to quickly test and tune abilities, game modes, enemies, and other gameplay systems during early development without having to build a custom debug window from scratch.

## Features

* Runtime debug window integrated with Unreal Engine
* Debug tabs and sections
* Buttons
* Toggles
* Spin Boxes
* Combo Boxes
* Float fields
* Text fields
* Configurable default tab and section
* Automatic creation of tabs and sections
* Configurable draw priority
* Blueprint accessible debug data
* Runtime control through a console command
* Lightweight subsystem-based architecture
* Designed for gameplay and system debugging
* No custom UMG widgets required
* Easy to extend with additional debug field types

## Requirements

* Unreal Engine 5.7.4 or later
* SlateIM

## Installation

Clone or download the repository into your project's `Plugins` folder.

Your project structure should look like:

    YourProject/
    ├── Content/
    ├── Source/
    ├── Plugins/
    │   └── RuntimeIMDebugs/
    └── YourProject.uproject

Compile your project and enable **RuntimeIMDebugs** from:

`Edit → Plugins`

## Configuration

RuntimeIMDebugs provides project settings under:

`Project Settings → Game → Runtime IM Debugs`

The default tab and section can be configured from the plugin settings.

These values are used whenever `NAME_None` is supplied as a tab or section ID.

<img width="1919" height="590" alt="image" src="https://github.com/user-attachments/assets/e330eb1d-547f-48a6-a24b-52ebaf9df2d4" />


## Architecture

RuntimeIMDebugs is built around a `UWorldSubsystem`.

Each active `UWorld` owns its own debug data, meaning tabs and sections belong to the lifetime of that world.

The main hierarchy is:

    URuntimeIMDebugsSubsystem
    │
    ├── DebugTab
    │   │
    │   ├── DebugSection
    │   │   ├── Button
    │   │   ├── Toggle
    │   │   ├── SpinBox
    │   │   ├── FloatField
    │   │   ├── ComboBox
    │   │   └── TextField
    │   │
    │   └── DebugSection
    │
    └── DebugTab

Tabs and sections can be created explicitly, but the `Add*` functions automatically create them when required.

### Tabs

Tabs are identified using an `FName`.

If `NAME_None` is passed as the tab ID, the configured default tab is used.

Tab IDs must be unique within the subsystem.

### Sections

Sections belong to a tab and are also identified using an `FName`.

If `NAME_None` is passed as the section ID, the configured default section is used.

Sections are ordered using their `DrawPriority`.

### Debug Fields

Debug fields are stored inside sections.

The current supported field types are:

* `Button`
* `Toggle`
* `SpinBox`
* `FloatField`
* `ComboBox`
* `TextField`

Each field has an `FName` ID that is used to access it after creation.

## API

The main API is exposed through `URuntimeIMDebugsSubsystem`.

### Debug Element Events

The main way to interact with RuntimeIMDebugs is through the events exposed by `URuntimeIMDebugsSubsystem`.

When a debug element is interacted with, the subsystem broadcasts its corresponding delegate. The callback receives the identifier of the debug element, allowing you to handle multiple elements from a single callback.

A typical setup is:

```cpp
URuntimeIMDebugsSubsystem* DebugSubsystem =
    GetWorld()->GetSubsystem<URuntimeIMDebugsSubsystem>();

DebugSubsystem->OnDebugButtonClicked.AddUObject(
    this,
    &AMyActor::OnDebugButtonClicked
);
```
<img width="1438" height="454" alt="image" src="https://github.com/user-attachments/assets/6eadc1cc-7a6b-4857-a8dc-5fc22d817400" />


Then identify the element inside the callback using its ID:

```cpp
void AMyActor::OnDebugButtonClicked(FName DebugID)
{
    if (DebugID == TEXT("ResetPlayer"))
    {
        ResetPlayer();
    }
    else if (DebugID == TEXT("KillAllEnemies"))
    {
        KillAllEnemies();
    }
}
```
<img width="1188" height="754" alt="image" src="https://github.com/user-attachments/assets/f2dad2d7-e43c-40c0-b36a-5a1fbdfff112" />

This allows multiple debug elements to share the same callback instead of requiring a separate function for every button or control.

The same approach is used for the other interactive debug elements, such as toggles, combo boxes, spin boxes, and other supported controls.

### Creating Debug Elements

Debug elements can be added directly through the subsystem:

    URuntimeIMDebugsSubsystem* DebugSubsystem =
        GetWorld()->GetSubsystem<URuntimeIMDebugsSubsystem>();

    DebugSubsystem->AddButton(
        TEXT("Gameplay"),
        TEXT("Player"),
        TEXT("ResetPlayer"),
        TEXT("Reset Player")
    );

    DebugSubsystem->AddToggle(
        TEXT("Gameplay"),
        TEXT("Player"),
        TEXT("GodMode"),
        TEXT("God Mode"),
        false
    );

    DebugSubsystem->AddFloatField(
        TEXT("Gameplay"),
        TEXT("Player"),
        TEXT("MovementSpeed"),
        TEXT("Movement Speed"),
        600.0f
    );

    DebugSubsystem->AddText(
        TEXT("Gameplay"),
        TEXT("Player"),
        TEXT("PlayerState"),
        TEXT("State"),
        TEXT("Idle")
    );

<img width="992" height="593" alt="image" src="https://github.com/user-attachments/assets/9a768372-039c-4214-ac43-1005e1f2f9fc" />


Tabs and sections do not need to be created beforehand. They are automatically created when required.

### Accessing Debug Elements

Existing debug elements can be accessed using their IDs:

    DebugSubsystem->SetToggleState(
        TEXT("Gameplay"),
        TEXT("Player"),
        TEXT("GodMode"),
        true
    );

    float MovementSpeed = DebugSubsystem->GetFloatFieldValue(
        TEXT("Gameplay"),
        TEXT("Player"),
        TEXT("MovementSpeed")
    );

### Default Tab and Section

Passing `NAME_None` uses the configured default tab or section:

    DebugSubsystem->AddFloatField(
        NAME_None,
        NAME_None,
        TEXT("MovementSpeed"),
        TEXT("Movement Speed"),
        600.0f
    );

This is equivalent to adding the field to the configured default tab and section.

### Tab Lookup

`GetTab()` performs a silent lookup and returns `nullptr` when the tab does not exist.

`GetTabChecked()` performs the same lookup but logs an error when the requested tab does not exist.

This distinction allows internal creation logic to perform expected lookups without producing unnecessary log messages.

## Console Command

RuntimeIMDebugs provides a console command for controlling the debug window:

    RuntimeIMDebugs.ShowWindow

With no argument, the window is toggled:

    RuntimeIMDebugs.ShowWindow

Show the window:

    RuntimeIMDebugs.ShowWindow 1

Hide the window:

    RuntimeIMDebugs.ShowWindow 0

## Debug Window

The debug window is controlled through the runtime subsystem:

    DebugSubsystem->ShowWindow();

    DebugSubsystem->HideWindow();

    DebugSubsystem->ToggleWindow();

The subsystem broadcasts window commands through its window command delegate, allowing the editor-side debug window to react without coupling the runtime API directly to the editor implementation.

## Extending RuntimeIMDebugs

RuntimeIMDebugs is designed to be extended with additional debug field types.

The existing field types follow the same general structure:

    Debug Field
        ├── ID
        ├── Label
        ├── Value
        └── DrawPriority

A new field type can follow the same pattern by:

1. Creating a debug field data structure.
2. Adding it to `FDebugSection`.
3. Adding an `Add*` function to `URuntimeIMDebugsSubsystem`.
4. Adding corresponding getter/setter functions when required.
5. Implementing its SlateIM rendering.
6. Connecting its runtime value changes to the subsystem API.

The existing `Button`, `Toggle`, `SpinBox`, `FloatField`, `ComboBox`, and `TextField` implementations provide examples of the intended architecture.

## Project Structure

    RuntimeIMDebugs/
    │
    ├── Config/
    │
    ├── Resources/
    │
    ├── Source/
    │   ├── RuntimeIMDebugs/
    │   │   ├── Public/
    │   │   └── Private/
    │   │
    │   └── RuntimeIMDebugsEditor/
    │       ├── Public/
    │       └── Private/
    │
    └── RuntimeIMDebugs.uplugin

The plugin is separated into Runtime and Editor modules.

The Runtime module contains the debug subsystem and public debugging API.

The Editor module is responsible for the editor-side debug window and its integration with the Unreal Editor.

## Supported Debug Field Types

| Field | Description |
|---|---|
| Button | Executes a debug action |
| Toggle | Enables or disables a boolean value |
| SpinBox | Adjusts a numeric value within a range |
| Float Field | Displays and modifies a floating-point value |
| Combo Box | Selects one option from a list |
| Text Field | Displays and modifies text |

## License

RuntimeIMDebugs is free and open-source software.

See the [LICENSE](LICENSE) file for license information.

## Support

If RuntimeIMDebugs is useful to you, consider supporting the project by giving it a ⭐ on GitHub.

Bug reports, feature requests, and contributions are welcome.
