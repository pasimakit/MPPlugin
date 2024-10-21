// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "SMenuMultiplayerWidget.generated.h"


struct FOnlineResultInformation;
class UCommonBorder;
enum class ECommonUserOnlineContext : uint8;
class UCommonSession_HostSessionRequest;
class USMenuJoinSessionWidget;
class USMenuHostSessionWidget;
class USButtonBaseWidget;

UCLASS(Abstract)
class MULTIPLAYERMENU_API USMenuMultiplayerWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;

protected:
	UFUNCTION()
	void OnUserOnlineLogin(const UCommonUserInfo* UserInfo, bool bSuccess, FText Error, ECommonUserPrivilege RequestedPrivilege, ECommonUserOnlineContext OnlineContext);
	
	UFUNCTION()
	void OnQuickPlayButtonClicked();
	
	UFUNCTION()
	void OnHostButtonClicked();

	UFUNCTION()
	void OnJoinButtonClicked();

	UFUNCTION()
	void OnExitButtonClicked();

	UFUNCTION()
	void QuickPlaySession();

	UCommonSession_HostSessionRequest* CreateQuickPlayHostSessionRequest();

	void OnJoinSessionComplete(const FOnlineResultInformation& Result) const;

	void SetSpinnerDisplay(const bool bDisplay) const;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USButtonBaseWidget* QuickPlayButton;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USButtonBaseWidget* HostButton;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USButtonBaseWidget* JoinButton;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USButtonBaseWidget* ExitButton;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
    TSoftClassPtr<USMenuJoinSessionWidget> JoinWidgetClass;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSoftClassPtr<USMenuHostSessionWidget> HostWidgetClass;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonBorder* SpinnerDisplay;
};
