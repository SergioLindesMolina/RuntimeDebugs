// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "RuntimeIMDebugsSettings.generated.h"

/**
 *
 */
UCLASS(Config = "Game", defaultconfig, meta = (DisplayName = "RuntimeIM Debugs"))
class RUNTIMEIMDEBUGS_API URuntimeIMDebugsSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	URuntimeIMDebugsSettings(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UPROPERTY(Config, EditAnywhere, Category = "General")
    FName DefaultTabName;

    UPROPERTY(Config, EditAnywhere, Category = "General")
    FName DefaultSectionName;

    UPROPERTY(Config, EditAnywhere, Category = "Section")
    FSlateBrush SectionBorderBrush;

    UPROPERTY(Config, EditAnywhere, Category = "Section")
    FSlateBrush SectionBorderHeaderBrush;



};
