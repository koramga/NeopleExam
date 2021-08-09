// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Button.h"
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
	UTextBlock* m_TBTotalProjectileCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"));
	TArray<UTextBlock*> m_TBProjectileCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"));
	UButton* m_BTTotalProjectileResetCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"));
	TArray<UButton*>	m_BTProjectileResetCount;

protected:
	virtual void NativePreConstruct();
	virtual void NativeConstruct();
	virtual void NativeDestruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

private :
	UFUNCTION()
	void TotalResetButtonClick();

	UFUNCTION()
	void NormalResetButtonClick();

	UFUNCTION()
	void ChargeResetButtonClick();

	UFUNCTION()
	void SplitResetButtonClick();

	UFUNCTION()
	void ReflectResetButtonClick();
	
private :
	UTextBlock* FindTextBlockFromProjectileType(EProjectileType ProjectileType);

public :
	void AddProjectileCount(EProjectileType ProjectileType, int Count);
	void ResetProjectileCount(EProjectileType ProjectileType);
	void ResetProjectileCount();
	void VisibleProgressBar(bool IsVisible);
	bool IsVisibleProgressBar() const;
	void SetProgressBarTime(float Time);
};
