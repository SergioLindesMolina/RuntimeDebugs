// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RuntimeIMDebugsSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDebugButtonPressed, const FName, ID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDebugToggleChanged, const FName, ID, bool, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDebugSliderChanged, const FName, ID, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDebugFloatFieldChanged, const FName, ID, float, Value);

//Used to order the difrent field given the draw priority and instertion order
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
    UPROPERTY(BlueprintReadOnly, Category = "DebugSlider")
    FName ID;

    UPROPERTY(BlueprintReadOnly, Category = "DebugSlider")
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

    FDebugFloatField(
        FName InID,
        const FString& InLable,
        const FString& InCachedString,
        float InValue,
        int InDrawPriority)
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
struct FDebugSlider 
{
    GENERATED_BODY()

    FDebugSlider()
        : Field(NAME_None, 0)
        , Label("")
        , Value(0.f)
    {
    }

    FDebugSlider(
        FName InID,
        const FString& InLable,
        float InValue,
        int InDrawPriority)
        : Field(InID, InDrawPriority)
        , Label(InLable)
        , Value(InValue)
    {
    }

    UPROPERTY(BlueprintReadOnly, Category = "DebugSlider")
    FDebugField Field;

    UPROPERTY(BlueprintReadOnly, Category = "DebugSlider")
    FString Label;

    UPROPERTY(BlueprintReadOnly, Category = "DebugSlider")
    float Value;
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

    FDebugToggle(
        FName InID,
        const FString& InLable,
        bool InValue,
        int InDrawPriority)
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

    FDebugButton(
        FName InID,
        const FString& InLable,
        int InDrawPriority)
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
struct FDebugTab
{
    GENERATED_BODY()

    FDebugTab() : ID(NAME_None), Label("") {}
    FDebugTab(FName InID, const FString& InLable) : ID(InID), Label(InLable){}

    /*Must be unique*/
    UPROPERTY(BlueprintReadOnly, Category = "DebugTab")
    FName ID;

    UPROPERTY(BlueprintReadOnly, Category = "DebugTab")
    FString Label;

    UPROPERTY(BlueprintReadOnly, Category = "DebugTab")
    TArray<FDebugButton> Buttons;
    UPROPERTY(BlueprintReadOnly, Category = "DebugTab")
    TArray<FDebugToggle> Toggles;
    UPROPERTY(BlueprintReadOnly, Category = "DebugTab")
    TArray<FDebugSlider> Sliders;
    UPROPERTY(BlueprintReadOnly, Category = "DebugTab")
    TArray<FDebugFloatField> FloatFields;

};


/**
 * 
 */
UCLASS()
class RUNTIMEIMDEBUGS_API URuntimeIMDebugsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

    //The variable and the value have the same name to be able to use that as a default value in blueprints functions
    const FName DefaultTab = TEXT("DefaultTab");

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem")
    void ShowWindow();

    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem")
    void HideWindow();

    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem")
    void ToggleWindow();

    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem")
    void AddTab(const FName InID, const FString& InLabel = TEXT(""));

    TArray<FDebugTab>& GetTabs();

    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem", meta=(InTabId = DefaultTab))
    void AddButton(const FName InTabID, const FName InID, const FString& InLabel = TEXT(""), int InDrawPriority = 0);

    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem", meta = (InTabId = DefaultTab))
    void AddToggle(const FName InTabID, const FName InID, const FString& InLabel = TEXT(""), bool InValue = false, int InDrawPriority = 0);
    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem", meta = (InTabId = DefaultTab))
    bool GetToggleState(const FName InTabID, const FName InID) const;
    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem", meta = (InTabId = DefaultTab))
    void SetToggleState(const FName InTabID, const FName InID, bool InValue);

    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem", meta = (InTabId = DefaultTab))
    void AddSlider(const FName InTabID, const FName InID, const FString& InLabel = TEXT(""), float InValue = 0.f, int InDrawPriority = 0);
    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem", meta = (InTabId = DefaultTab))
    float GetSliderValue(const FName InTabID, const FName InID) const;
    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem", meta = (InTabId = DefaultTab))
    void SetSliderValue(const FName InTabID, const FName InID, float InValue);

    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem", meta = (InTabId = DefaultTab))
    void AddFloatField(const FName InTabID , const FName InID, const FString& InLabel = TEXT(""), float InValue = 0.f, int InDrawPriority = 0);
    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem", meta = (InTabId = DefaultTab))
    float GetFloatFieldValue(const FName InTabID, const FName InID) const;
    UFUNCTION(BlueprintCallable, Category = "RuntimeDebugsSubsystem", meta = (InTabId = DefaultTab))
    void SetFloatFieldValue(const FName InTabID, const FName InID, float InValue);
    
    UPROPERTY(BlueprintAssignable, Category = "RuntimeDebugsSubsystem")
    FOnDebugButtonPressed OnButtonPressed;
    UPROPERTY(BlueprintAssignable, Category = "RuntimeDebugsSubsystem")
    FOnDebugToggleChanged OnToggleChanged;
    UPROPERTY(BlueprintAssignable, Category = "RuntimeDebugsSubsystem")
    FOnDebugSliderChanged OnSliderChanged;
    UPROPERTY(BlueprintAssignable, Category = "RuntimeDebugsSubsystem")
    FOnDebugFloatFieldChanged OnDebugFloatFieldChanged;

protected:

    FDebugTab* GetTab(const FName InID);

    const FDebugTab* GetTab(const FName InID) const;

    UPROPERTY(BlueprintReadOnly, Category = "RuntimeDebugsSubsystem")
    TArray<FDebugTab> Tabs;
    
};
