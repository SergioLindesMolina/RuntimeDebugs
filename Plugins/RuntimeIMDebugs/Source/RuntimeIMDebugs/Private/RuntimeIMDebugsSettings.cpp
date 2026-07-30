// Fill out your copyright notice in the Description page of Project Settings.


#include "RuntimeIMDebugsSettings.h"

URuntimeIMDebugsSettings::URuntimeIMDebugsSettings(const FObjectInitializer& ObjectInitializer)
{
	
	DefaultTabName = TEXT("DefaultTab");
	DefaultSectionName = TEXT("DefaultSection");


    // General Section Border
    SectionBorderBrush = FSlateBrush();
    SectionBorderBrush.TintColor = FSlateColor(FLinearColor(0.276042f, 0.276042f, 0.276042f, 0.8f));

    SectionBorderBrush.DrawAs = ESlateBrushDrawType::Border;
    SectionBorderBrush.Tiling = ESlateBrushTileType::NoTile;
    SectionBorderBrush.Mirroring = ESlateBrushMirrorType::NoMirror;
    SectionBorderBrush.ImageSize = FVector2D(32.0f, 32.0f);
    SectionBorderBrush.Margin = FMargin(1.0f);


    // Header Section Border
    SectionBorderHeaderBrush = FSlateBrush();
    SectionBorderHeaderBrush.TintColor = FSlateColor(FLinearColor(0.34375f, 0.34375f, 0.34375f, 1.0f));

    SectionBorderHeaderBrush.DrawAs = ESlateBrushDrawType::Border;
    SectionBorderHeaderBrush.Tiling = ESlateBrushTileType::NoTile;
    SectionBorderHeaderBrush.Mirroring = ESlateBrushMirrorType::NoMirror;
    SectionBorderHeaderBrush.ImageSize = FVector2D(32.0f, 32.0f);
    SectionBorderHeaderBrush.Margin = FMargin(1.0f);

}