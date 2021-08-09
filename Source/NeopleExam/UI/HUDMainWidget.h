// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Blueprint/UserWidget.h"
#include "HUDMainWidget.generated.h"

/**
 * 
 */
UCLASS()
class NEOPLEEXAM_API UHUDMainWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"));
	UProgressBar* m_PBShotTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"));
	UTextBlock* m_TBShotCount;

protected:
	virtual void NativePreConstruct();
	virtual void NativeConstruct();
	virtual void NativeDestruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
	
public :
	void AddShotCount(int Count);
	void ResetShotCount();
	void VisibleProgressBar(bool IsVisible);
	bool IsVisibleProgressBar() const;
	void SetProgressBarTime(float Time);
};
