// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/CanvasRenderTarget2D.h"
#include "AC_Paint_Ratio.generated.h"

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_DELEGATE_OneParam(FPaintRatio, double, OutRatio);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PAINT_RATIO_API UAC_Paint_Ratio : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAC_Paint_Ratio();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadWrite)
	UCanvasRenderTarget2D* CRT_Drawing = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (ToolTip = "RGBA values should between 0-255"))
	FColor AlphaColor = FColor::White;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	virtual void PrepareRenderTarget();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Record Painted Pixels", ToolTip = "It records all painted pixels' index and color values. It contains all markers and color values are in FColor (0-255) format.", Keywords = "record, paint, painted, color, pixel, render, target, canvas, ratio"), Category = "Window System|Export")
	virtual void RecordPaintedPixels();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Color Ratio", ToolTip = "It filters and gives count of only wanted color.", Keywords = "get, painted, pixel, pixels, color, render, target, ratio"), Category = "Window System|Export")
	virtual void GetColorRatio(FColor WantedColor, FPaintRatio DelegatePaintRatio);

	TMap<FString, FColor> Painted_Pixels;
	
	double Pixel_Count = 0;

};