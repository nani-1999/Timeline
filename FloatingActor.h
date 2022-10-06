// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingActor.generated.h"

UCLASS()
class AKTOR_API AFloatingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatingActor();

	UPROPERTY()
	class UStaticMeshComponent* Mesh;
	UPROPERTY()
	class UTimelineComponent* Transition;
	UPROPERTY()
	class UCurveFloat* fCurve;

	UFUNCTION()
	void UpdateFunction(float Value);
	UFUNCTION()
	void FinishedFunction();

	FVector ActorInitialLocation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};