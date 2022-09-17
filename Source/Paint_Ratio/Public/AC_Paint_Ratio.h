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

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void PrepareRenderTarget();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Record Painted Pixels", ToolTip = "It records all painted pixels' index and color values. \"AlphaColor\" input uses FColor so, RGB-A values should be 0-255 format. There should be at least 1 second delay between \"Record\" and \"Get Ratio\" functions. Canvas render target format should use RTF RGBA8 and Address X-Y should use \"Clamp\".", Keywords = "record, paint, painted, color, pixel, render, target, canvas, ratio"), Category = "CRT Paint Ratio")
	virtual void RecordPaintedPixels(FColor AlphaColor = FColor::White);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Color Ratio", ToolTip = "It filters painted pixel map and gives only wanted color's ratio.", Keywords = "get, painted, pixel, pixels, color, render, target, ratio"), Category = "CRT Paint Ratio")
	virtual void GetColorRatio(FPaintRatio DelegatePaintRatio, FColor WantedColor = FColor::White);

	UPROPERTY()
	TMap<FString, FColor> Painted_Pixels;	// UPROPOERTY Global variable. It records whole painted pixels with all markers.
	
	UPROPERTY()
	double Pixel_Count = 0;					// UPROPERTY Global Variable. Pixel count of canvas render target. We use this to calculate ratio.

};