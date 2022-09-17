// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/CanvasRenderTarget2D.h"
#include "Math/Color.h"

#include "AC_Paint_Ratio.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PAINT_RATIO_API UAC_Paint_Ratio : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAC_Paint_Ratio();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	virtual void ClearRenderTarget();

	//UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Painted Pixels", ToolTip = "Tooltip.", Keywords = "get, painted, pixel, pixels, render, target, ratio"), Category = "Window System|Export")
	virtual void GetPaintedPixels();

	TMap<FString, FColor> Painted_Pixels;


public:	

	UPROPERTY(BlueprintReadWrite)
	UCanvasRenderTarget2D* CRT_Drawing = nullptr;
	
	UPROPERTY(BlueprintReadWrite)
	FColor AlphaColor = FColor::White;
	
	UPROPERTY(BlueprintReadWrite)
	double ReadInterval = 2;

	UPROPERTY(BlueprintReadOnly)
	double OutRatio = 0;

	UPROPERTY(BlueprintReadWrite)
	bool bAllowDebugMessage = true;
	
};
