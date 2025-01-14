﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "SGameDisplayListWidget.generated.h"

class UVerticalBox;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameListCreatedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameSelectedDelegate, const FPrimaryAssetId&, SelectedGameData);
UCLASS(Abstract)
class MULTIPLAYERMENU_API USGameDisplayListWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	TArray<UWidget*> GetGameListWidgetList() const;

	FOnGameListCreatedDelegate OnGameListCreated;
	FOnGameSelectedDelegate OnGameSelected;

protected:
	void LoadGameList();
	void OnGameDataLoaded(TArray<FPrimaryAssetId> GameDataAssets);
	void OnGameListCreation();

	UFUNCTION()
	void OnGameDisplaySelection(const FPrimaryAssetId& SelectedGameData);
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UVerticalBox* GameList;

	UPROPERTY()
	UCommonUserWidget* GameDisplayWidgetClass;
};