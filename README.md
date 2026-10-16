# RuntimeIMDebugs

<img width="1125" height="657" alt="image" src="https://github.com/user-attachments/assets/f3aaff17-ec82-4b9f-8327-2e14a9f5ae1a" />


## Contents

- [About](#about)
- [Features](#features)
- [Requirements](#requirements)
- [Supported Execution Modes](#supported-execution-modes)
- [Installation](#installation)
- [Configuration](#configuration)
- [Architecture](#architecture)
- [API](#api)
  - [Creating Debug Elements](#creating-debug-elements)
  - [Accessing Debug Elements](#accessing-debug-elements)
  - [Debug Element Events](#debug-element-events)
  - [Default Tab and Section](#default-tab-and-section)
- [Console Command](#console-command)
- [Debug Window](#debug-window)
- [Extending RuntimeIMDebugs](#extending-runtimeimdebugs)
- [Project Structure](#project-structure)
- [Supported Debug Field Types](#supported-debug-field-types)
- [License](#license)
- [Support](#support)

## About

RuntimeIMDebugs is a free and open-source Unreal Engine plugin that provides a lightweight debug window for exposing gameplay and system information during development.

The plugin allows programmers and designers to create debug interfaces directly from C++ without having to create custom UMG widgets or editor tools.

RuntimeIMDebugs is built on top of SlateIM and is designed for fast iteration, gameplay debugging, parameter tuning, and temporary runtime instrumentation.

The main goal of RuntimeIMDebugs is to provide a simple way to create a small debug window for testing and adjusting gameplay systems that are difficult to handle with simple log messages or `PrintString`. It can be used to quickly test and tune abilities, game modes, enemies, and other gameplay systems without having to build a custom debug window from scratch.

**RuntimeIMDebugs is intended primarily for Play In Editor (PIE) sessions running inside the Unreal Editor viewport.** It is designed to work with single-player gameplay and multiplayer testing using a listen server.

The plugin can also work with client worlds, although world selection may be less predictable in some configurations.

The Unreal Editor's Standalone Play Mode is not currently supported. The plugin is intended for editor-integrated development and debugging, rather than for providing a production debugging interface for packaged games.

❤️ Support the Project

RuntimeIMDebugs is free to use and extend. If you find it useful and would like to support its development and future improvements, you can make a voluntary donation.

Donations are entirely optional. All plugin features remain available without donating.

Donate: 

Thank you for supporting the project!

🎥 Video Tutorials

Learn how to  use RuntimeIMDebugs through video tutorials.

Getting Started: 

Blueprint Use: 

C++ Use:

## Features

- Runtime debug window integrated with Unreal Editor
- Debug tabs and sections
- Buttons
- Toggles
- Spin boxes
- Combo boxes
- Float fields
- Text fields
- Configurable default tab and section
- Automatic creation of tabs and sections
- Configurable draw priority
- Blueprint-accessible debug data
- Runtime control through a console command
- World subsystem-based architecture
- No custom UMG widgets required
- Extendable architecture for additional debug field types

## Requirements

- Unreal Engine 5.7.4 or later
- SlateIM

SlateIM is a required plugin dependency. Make sure it is available and enabled in your project.

## Supported Execution Modes

RuntimeIMDebugs is designed around an editor-integrated debugging workflow.

### Play In Editor (PIE)

The intended way to use RuntimeIMDebugs is to play directly inside the Unreal Editor viewport using PIE.

This includes single-player gameplay and multiplayer testing with a listen server.

### PIE with Listen Server

Listen server gameplay is a preferred workflow for multiplayer testing. The plugin can locate an appropriate world for its debug interface while excluding dedicated server worlds.

However, when multiple worlds are available, the plugin's automatic world-selection behavior determines which world's debug data is displayed.

### PIE Clients

The plugin can also work with a client world. However, client selection is automatic, and the plugin does not currently provide a way to choose a specific client manually.

When multiple worlds are available, the plugin prefers the first eligible client world it finds. Depending on the session configuration, this behavior may not always select the client you intended.

For this reason, PIE inside the editor viewport, particularly single-player gameplay and listen server workflows, is the recommended way to use the plugin.

### Standalone Play Mode

The Unreal Editor's **Standalone** Play Mode is not currently supported.

This is different from playing inside the editor viewport using PIE. Both modes allow you to test gameplay, but RuntimeIMDebugs is designed for the editor-integrated workflow and is not intended to operate in Standalone Play Mode.

### World Selection

The debug window searches the available Unreal Engine world contexts to find a suitable world.

The selection process follows these rules:

1. Worlds that are invalid or whose type is not `PIE` or `Game` are ignored.
2. Dedicated server worlds and worlds with `NM_MAX` net mode are ignored.
3. The first eligible world is retained as a fallback.
4. If an eligible client world is found, it takes precedence over the fallback world.
5. If multiple eligible client worlds exist, the first one encountered is selected.

World selection is automatic. The plugin does not currently support selecting a world or multiplayer client through the debug window.

### Unsupported Modes

The current version is not intended or supported for:

- The Unreal Editor's Standalone Play Mode.
- Shipping builds.
- Standalone applications running independently of the Unreal Editor.

The plugin is intended for development and debugging inside the editor, rather than for providing a production debugging interface to end users.

In the future, it may be possible to add support for these execution modes and to allow users to select a specific client during multiplayer sessions. These capabilities are not currently implemented.

## Installation

Clone or download the repository into your project's `Plugins` folder.

Your project structure should look like:

```
YourProject/
├── Content/
├── Source/
├── Plugins/
│   └── RuntimeIMDebugs/
└── YourProject.uproject
```

Make sure the required SlateIM dependency is available and enabled.

Compile your project and enable RuntimeIMDebugs from:

`Edit → Plugins`

Restart the editor if required by the plugin installation or compilation process.

## Configuration

RuntimeIMDebugs provides project settings under:

`Project Settings → Game → Runtime IM Debugs`

The following settings are available:

- `DefaultTabName`: Default tab identifier.
- `DefaultSectionName`: Default section identifier.
- `SectionBorderBrush`: Appearance of the section border.
- `SectionBorderHeaderBrush`: Appearance of the section header border.

The default identifiers are `DefaultTab` and `DefaultSection`.

These values are used whenever `NAME_None` is supplied as a tab or section ID.

The border brushes can be used to adjust the appearance of debug sections.

<img width="1704" height="707" alt="image" src="https://github.com/user-attachments/assets/31f986ab-d954-4bf0-90b6-4656994ac705" />

## Architecture

RuntimeIMDebugs is built around a `UWorldSubsystem`.

Each active `UWorld` owns its own debug data, meaning tabs and sections belong to the lifetime of that world.

The main hierarchy is:

```text
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
```

Tabs and sections can be created explicitly, but the `Add*` functions automatically create them when required.

### Tabs

Tabs are identified using an `FName`.

If `NAME_None` is passed as the tab ID, the configured default tab is used.

Tab IDs must be unique within the subsystem. If a tab with the requested ID already exists, `AddTab` returns its existing index and logs an error.

### Sections

Sections belong to a tab and are also identified using an `FName`.

If `NAME_None` is passed as the section ID, the configured default section is used.

Sections are ordered using their `DrawPriority`. Lower values are drawn first.

### Debug Fields

Debug fields are stored inside sections.

The currently supported field types are:

- `Button`
- `Toggle`
- `SpinBox`
- `FloatField`
- `ComboBox`
- `TextField`

Each field has an `FName` ID used to access it after creation. Field IDs must be unique within their section.

Debug fields can also have a `DrawPriority`, which determines their order within the section. Lower values are drawn first.

## API

The main API is exposed through `URuntimeIMDebugsSubsystem`.

You can obtain the subsystem from a world:

```cpp
URuntimeIMDebugsSubsystem* DebugSubsystem =
    GetWorld()->GetSubsystem<URuntimeIMDebugsSubsystem>();
```

Make sure you use the world whose debug data you want to access.

### Creating Debug Elements

Debug elements can be added directly through the subsystem:

```cpp
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
```
<img width="809" height="340" alt="image" src="https://github.com/user-attachments/assets/ad605416-b1b3-4de3-9de7-c069e0371f1a" />


The parameters generally identify the tab, section, field, and display label, followed by the initial value where applicable.

Tabs and sections do not need to be created beforehand. They are automatically created when required by the `Add*` functions.

The `InDrawPriority` parameter can be used to control the order in which a section or debug field is displayed.

### Accessing Debug Elements

Existing debug elements can be accessed using their IDs.

For example, to change the state of a toggle:

```cpp
DebugSubsystem->SetToggleState(
    TEXT("Gameplay"),
    TEXT("Player"),
    TEXT("GodMode"),
    true
);
```

<img width="691" height="539" alt="image" src="https://github.com/user-attachments/assets/36d114c2-0981-46dd-966e-e0fa8f2032d8" />


To read the value of a float field:

```cpp
float MovementSpeed = DebugSubsystem->GetFloatFieldValue(
    TEXT("Gameplay"),
    TEXT("Player"),
    TEXT("MovementSpeed")
);
```

<img width="741" height="413" alt="image" src="https://github.com/user-attachments/assets/ebc0bde1-8e29-4079-bcea-ffc3acc2708b" />


The subsystem also provides getter and setter functions for supported controls where applicable.

These functions use the tab, section, and field IDs to identify the element.

### Debug Element Events

The subsystem exposes delegates that notify you when a user interacts with a debug element.

The available events are:

| Delegate | Parameters |
|---|---|
| `OnButtonPressed` | Tab ID, section ID, field ID |
| `OnToggleChanged` | Tab ID, section ID, field ID, boolean value |
| `OnSpinBoxChanged` | Tab ID, section ID, field ID, float value |
| `OnDebugFloatFieldChanged` | Tab ID, section ID, field ID, float value |
| `OnDebugComboBoxChanged` | Tab ID, section ID, field ID, selected index |

These delegates are exposed as Blueprint-assignable events.

<img width="1104" height="540" alt="image" src="https://github.com/user-attachments/assets/d1c0c8d4-c5e9-45bf-ba1d-9064da84cbce" />

<img width="866" height="419" alt="image" src="https://github.com/user-attachments/assets/291c04a5-af85-4feb-a8dc-73cfb29f036b" />


For example, a button event can be handled in C++ using a `UFUNCTION` and `AddDynamic`:

```cpp
// In your class declaration:

UFUNCTION()
void HandleDebugButtonPressed(
    FName TabID,
    FName SectionID,
    FName ID
);
```

Bind the event:

```cpp
DebugSubsystem->OnButtonPressed.AddDynamic(
    this,
    &AMyActor::HandleDebugButtonPressed
);
```

Then handle the interaction:

```cpp
void AMyActor::HandleDebugButtonPressed(
    FName TabID,
    FName SectionID,
    FName ID)
{
    if (TabID == TEXT("Gameplay") &&
        SectionID == TEXT("Player") &&
        ID == TEXT("ResetPlayer"))
    {
        ResetPlayer();
    }
}
```

This allows multiple debug elements to share a callback instead of requiring a separate function for every button.

The other interactive controls follow the same general approach, but their delegates also provide the relevant changed value or selected index.

Use the exact delegate signature for the control you want to handle.

### Default Tab and Section

Passing `NAME_None` uses the configured default tab or section:

```cpp
DebugSubsystem->AddFloatField(
    NAME_None,
    NAME_None,
    TEXT("MovementSpeed"),
    TEXT("Movement Speed"),
    600.0f
);
```

This is equivalent to adding the field to the configured default tab and section.

## Console Command

RuntimeIMDebugs provides a console command for controlling the debug window:

```text
RuntimeIMDebugs.ShowWindow
```

With no argument, the window is toggled:

```text
RuntimeIMDebugs.ShowWindow
```

Show the window:

```text
RuntimeIMDebugs.ShowWindow 1
```

Hide the window:

```text
RuntimeIMDebugs.ShowWindow 0
```

The command uses Unreal Engine's current play world to find the corresponding `URuntimeIMDebugsSubsystem`.

If the engine, current play world, or subsystem is unavailable, the command does nothing.

## Debug Window

The debug window is controlled through the subsystem:

<img width="1632" height="514" alt="image" src="https://github.com/user-attachments/assets/d00616d8-031d-476a-a99d-f79744b1aad1" />

```cpp
DebugSubsystem->ShowWindow();

DebugSubsystem->HideWindow();

DebugSubsystem->ToggleWindow();
```

These functions broadcast window commands when compiled with `WITH_EDITOR`.

The editor module handles those commands and manages the dockable debug window. This keeps the subsystem's debug data separate from the editor-side UI implementation.

The window searches the available world contexts to determine which world's debug data to display. Client worlds are preferred over the first eligible fallback world.

## Extending RuntimeIMDebugs

RuntimeIMDebugs is designed to be extended with additional debug field types.

The existing field types follow the same general structure:

```text
Debug Field
    ├── ID
    ├── Label
    ├── Value
    └── DrawPriority
```

A new field type can follow the same pattern by:

1. Creating a debug field data structure.
2. Adding it to `FDebugSection`.
3. Adding an `Add*` function to `URuntimeIMDebugsSubsystem`.
4. Adding corresponding getter/setter functions when required.
5. Implementing its SlateIM rendering.
6. Connecting its runtime value changes to the subsystem API.

The existing `Button`, `Toggle`, `SpinBox`, `FloatField`, `ComboBox`, and `TextField` implementations provide examples of the intended architecture.

## Project Structure

```text
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
```

The plugin is separated into two modules.

The `RuntimeIMDebugs` module contains the subsystem and debugging API.

The `RuntimeIMDebugsEditor` module is responsible for the editor-side debug window and its integration with Unreal Editor.

The plugin depends on SlateIM for its immediate-mode UI.

## Supported Debug Field Types

| Field | Description |
|---|---|
| Button | Triggers a debug action |
| Toggle | Enables or disables a boolean value |
| SpinBox | Adjusts a numeric value within a range |
| Float Field | Displays and modifies a floating-point value |
| Combo Box | Selects an option from a list |
| Text Field | Displays text |

## License

RuntimeIMDebugs is free and open-source software.

See the [LICENSE](LICENSE.txt) file for license information.

## Support

If RuntimeIMDebugs is useful to you, consider supporting the project by giving it a ⭐ on GitHub.

Bug reports, feature requests, and contributions are welcome.
