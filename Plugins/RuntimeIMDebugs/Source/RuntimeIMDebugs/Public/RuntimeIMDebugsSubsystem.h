// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RuntimeIMDebugsSubsystem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDebugButtonPressed, const FName, ID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDebugToggleChanged, const FName, ID, bool, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDebugSpinBoxChanged, const FName, ID, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDebugFloatFieldChanged, const FName, ID, float, Value);

enum class ERuntimeIMDebugWindowCommand : uint8
{
    Show,
    Hide,
    Toggle
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnWindowCommand, ERuntimeIMDebugWindowCommand);


//Used to order the difrent field given the draw priority 
struct FDebugFieldDrawListEntry 
{
    FDebugFieldDrawListEntry() : DrawPriority(0) {}
    FDebugFieldDrawListEntry(
        int InDrawPriority, 
        TFunction<void()> InDrawFunction)
        : DrawPriority(InDrawPriority), 
        DrawFunction(MoveTemp(InDrawFunction))
    {}

    int32 DrawPriority;

    TFunction<void()> DrawFunction;
};

USTRUCT(BlueprintType)
struct FDebugField
{
    GENERATED_BODY()

    FDebugField() 
        : ID(NAME_None), 
        DrawPriority(0) 
    {}
    
    FDebugField(
        FName InID, 
        int InDrawPriority = 0) 
        : ID(InID), 
        DrawPriority(InDrawPriority) 
    {}

    /*Must be unique*/
    UPROPERTY(BlueprintReadOnly, Category = "DebugField")
    FName ID;

    UPROPERTY(BlueprintReadOnly, Category = "DebugField")
    int DrawPriority;
};

USTRUCT(BlueprintType)
struct FDebugFloatField 
{
    GENERATED_BODY()

    FDebugFloatField()
        : Field(NAME_None, 0)
        , Label("")
        , CachedStringAsValue(FString::SanitizeFloat(0))
        , Value(0.f)
    {
    }

    FDebugFloatField(FName InID, const FString& InLable, const FString& InCachedString, float InValue, int InDrawPriority)
        : Field(InID, InDrawPriority)
        , Label(InLable)
        , CachedStringAsValue(InCachedString)
        , Value(InValue)
    {
    }

    UPROPERTY(BlueprintReadOnly, Category = "DebugFloatField")
    FDebugField Field;

    UPROPERTY(BlueprintReadOnly, Category = "DebugFloatField")
    FString Label;

    UPROPERTY(BlueprintReadOnly, Category = "DebugFloatField")
    FString CachedStringAsValue;

    UPROPERTY(BlueprintReadOnly, Category = "DebugFloatField")
    float Value;
};

USTRUCT(BlueprintType)
struct FDebugSpinBox 
{
    GENERATED_BODY()

    FDebugSpinBox()
        : Field(NAME_None, 0)
        , Label("")
        , Value(0.f)
        , Min(0.f)
        , Max(1.0f)
    {
    }

    FDebugSpinBox(FName InID, const FString& InLable, float InValue, float InMin, float InMax, int InDrawPriority)
        : Field(InID, InDrawPriority)
        , Label(InLable)
        , Value(InValue)
        , Min(InMin)
        , Max(InMax)
    {
    }

    UPROPERTY(BlueprintReadOnly, Category = "DebugSpinBox")
    FDebugField Field;

    UPROPERTY(BlueprintReadOnly, Category = "DebugSpinBox")
    FString Label;

    UPROPERTY(BlueprintReadOnly, Category = "DebugSpinBox")
    float Value;

    UPROPERTY(BlueprintReadOnly, Category = "DebugSpinBox")
    float Min;
    
    UPROPERTY(BlueprintReadOnly, Category = "DebugSpinBox")
    float Max;
};

USTRUCT(BlueprintType)
struct FDebugToggle
{
    GENERATED_BODY()

    FDebugToggle()
        : Field(NAME_None, 0)
        , Label("")
        , Value(false)
    {
    }

    FDebugToggle(FName InID, const FString& InLable, bool InValue, int InDrawPriority)
        : Field(InID, InDrawPriority)
        , Label(InLable)
        , Value(InValue)
    {
    }

    UPROPERTY(BlueprintReadOnly, Category = "DebugToggle")
    FDebugField Field;

    UPROPERTY(BlueprintReadOnly, Category = "DebugToggle")
    FString Label;

    UPROPERTY(BlueprintReadOnly, Category = "DebugToggle")
    bool Value;

};

USTRUCT(BlueprintType)
struct FDebugButton 
{
    GENERATED_BODY()

    FDebugButton()
        : Field(NAME_None, 0)
        , Label("")
    {
    }

    FDebugButton(FName InID, const FString& InLable, int InDrawPriority)
        : Field(InID, InDrawPriority)
        , Label(InLable)
    {
    }

    UPROPERTY(BlueprintReadOnly, Category = "DebugButton")
    FDebugField Field;

    UPROPERTY(BlueprintReadOnly, Category = "DebugButton")
    FString Label;

};

USTRUCT(BlueprintType)
struct FDebugSection
{
    GENERATED_BODY()

    FDebugSection()
        : Field(NAME_None, 0),
        Label("Default")
    {
    }

    FDebugSection(FName InID, const FString InLabel, int InDrawPriority)
        : Field(InID, InDrawPriority),
        Label(InLabel)
    {
    }


    UPROPERTY(BlueprintReadOnly, Category = "DebugSection")
    FDebugField Field;

    UPROPERTY(BlueprintReadOnly, Category = "DebugSection")
    FString Label;

    UPROPERTY(BlueprintReadOnly, Category = "DebugSection")
    TArray<FDebugButton> Buttons;
    UPROPERTY(BlueprintReadOnly, Category = "DebugSection")
    TArray<FDebugToggle> Toggles;
    UPROPERTY(BlueprintReadOnly, Category = "DebugSection")
    TArray<FDebugSpinBox> SpinBoxes;
    UPROPERTY(BlueprintReadOnly, Category = "DebugSection")
    TArray<FDebugFloatField> FloatFields;

};

USTRUCT(BlueprintType)
struct FDebugTab
{
    GENERATED_BODY()

    FDebugTab() 
        : ID(NAME_None), 
        Label("") 
    {}
    
    FDebugTab(FName InID, const FString& InLable) 
        : ID(InID), 
        Label(InLable)
    {}

    /*Must be unique*/
    UPROPERTY(BlueprintReadOnly, Category = "DebugTab")
    FName ID;

    UPROPERTY(BlueprintReadOnly, Category = "DebugTab")
    FString Label;

    UPROPERTY(BlueprintReadOnly, Category = "DebugTab")
    TArray<FDebugSection> DebugSections;

};


/**
 * 
 */
UCLASS()
class RUNTIMEIMDEBUGS_API URuntimeIMDebugsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

    UPROPERTY(BlueprintReadWrite)
    FSlateBrush TestBrush;

    UPROPERTY(BlueprintReadWrite)
    FSlateBrush TestBrush2;



    inline static FOnWindowCommand OnWindowCommand;

    //The variable and the value have the same name to be able to use that as a default value in blueprints functions
    const FName DefaultTab = TEXT("DefaultTab");
    const FName DefaultSection = TEXT("DefaultSection");

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem")
    void ShowWindow();

    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem")
    void HideWindow();

    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem")
    void ToggleWindow();

    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem")
    int AddTab(const FName InID, const FString& InLabel = TEXT(""));

    TArray<FDebugTab>& GetTabs();

    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem")
    int AddDebugSection(const FName InTabID, const FName InID, const FString& InLabel = TEXT(""), int InDrawPriority = 0);

    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem", meta=(InTabId = DefaultTab))
    void AddButton(const FName InTabID, const FName InSectionID, const FName InID, const FString& InLabel = TEXT(""), int InDrawPriority = 0);

    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem", meta = (InTabId = DefaultTab))
    void AddToggle(const FName InTabID, const FName InSectionID, const FName InID, const FString& InLabel = TEXT(""), bool InValue = false, int InDrawPriority = 0);
    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem", meta = (InTabId = DefaultTab))
    bool GetToggleState(const FName InTabID, const FName InSectionID, const FName InID) const;
    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem", meta = (InTabId = DefaultTab))
    void SetToggleState(const FName InTabID, const FName InSectionID, const FName InID, bool InValue);

    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem", meta = (InTabId = DefaultTab))
    void AddSpinBox(const FName InTabID, const FName InSectionID, const FName InID, const FString& InLabel = TEXT(""), float InValue = 0.f,
        float InMin = 0.0f, float InMax = 1.0f, int InDrawPriority = 0);
    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem", meta = (InTabId = DefaultTab))
    float GetSpinBoxValue(const FName InTabID, const FName InSectionID, const FName InID) const;
    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem", meta = (InTabId = DefaultTab))
    void SetSpinBoxValue(const FName InTabID, const FName InSectionID, const FName InID, float InValue);

    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem", meta = (InTabId = DefaultTab))
    void AddFloatField(const FName InTabID , const FName InSectionID, const FName InID, const FString& InLabel = TEXT(""), float InValue = 0.f, int InDrawPriority = 0);
    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem", meta = (InTabId = DefaultTab))
    float GetFloatFieldValue(const FName InTabID, const FName InSectionID, const FName InID) const;
    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem", meta = (InTabId = DefaultTab))
    void SetFloatFieldValue(const FName InTabID, const FName InSectionID, const FName InID, float InValue);
    
    UPROPERTY(BlueprintAssignable, Category = "RuntimeDebugsSubsystem")
    FOnDebugButtonPressed OnButtonPressed;
    UPROPERTY(BlueprintAssignable, Category = "RuntimeDebugsSubsystem")
    FOnDebugToggleChanged OnToggleChanged;
    UPROPERTY(BlueprintAssignable, Category = "RuntimeDebugsSubsystem")
    FOnDebugSpinBoxChanged OnSpinBoxChanged;
    UPROPERTY(BlueprintAssignable, Category = "RuntimeDebugsSubsystem")
    FOnDebugFloatFieldChanged OnDebugFloatFieldChanged;

protected:

    FDebugTab* GetTab(const FName InID);

    const FDebugTab* GetTab(const FName InID) const;

    FDebugSection* GetDebugSection(const FName InTabID, const FName InID);

    FDebugSection* GetDebugSection(FDebugTab* InTab, const FName InID);
    
    const FDebugSection* GetDebugSection(const FName InTabID, const FName InID) const;

    FDebugTab* GetOrCreateTab(const FName InTabID);

    FDebugSection* GetOrCreateDebugSection(const FName InTabID, const FName InSectionID);

    const FName ResolveTabID(const FName InTabID) const;

    const FName ResolveDebugSectionID(const FName InSectionID) const;

    UPROPERTY(BlueprintReadOnly, Category = "RuntimeDebugsSubsystem")
    TArray<FDebugTab> Tabs;
    
};