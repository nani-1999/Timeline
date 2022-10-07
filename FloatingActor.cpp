// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AFloatingActor::AFloatingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_TriPyramid.Shape_TriPyramid'"));
	if (StaticMeshAsset.Succeeded()) {
		Mesh->SetStaticMesh(StaticMeshAsset.Object);
	}

	Transition = CreateDefaultSubobject<UTimelineComponent>(TEXT("Transition"));

	static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveFloatAsset(TEXT("CurveFloat'/Game/Blueprint/FloatingCurveFloat.FloatingCurveFloat'"));
	if (CurveFloatAsset.Succeeded()) {
		UE_LOG(LogTemp, Warning, TEXT("Getting Curve Successful"));
		fCurve = CurveFloatAsset.Object;
	}

	ActorInitialLocation = FVector::ZeroVector;

	//actor must be 'movable' for transition to work
}

// Called when the game starts or when spawned
void AFloatingActor::BeginPlay()
{
	Super::BeginPlay();

	ActorInitialLocation = GetActorLocation();

	if (fCurve) {
		FOnTimelineFloat UpdateBind;
		UpdateBind.BindDynamic(this, &AFloatingActor::UpdateZOffset);
		Transition->AddInterpFloat(fCurve, UpdateBind, FName("Alpha"));

		FOnTimelineEvent FinishedBind;
		FinishedBind.BindDynamic(this, &AFloatingActor::FinishedFunc);
		Transition->SetTimelineFinishedFunc(FinishedBind);
	}
	//Transition->SetLooping(false);    //loops, but always starts from the beginning on each loop
	//Transition->SetIgnoreTimeDilation(true);    //ignores world time, means ignores if world is slow motion.
	Transition->Play();
}

// Called every frame
void AFloatingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFloatingActor::UpdateZOffset(float Value) {
	SetActorLocation(ActorInitialLocation + FVector(0.f, 0.f, Value));
}
void AFloatingActor::FinishedFunc() {
	if (Transition->GetPlaybackPosition() == 0.f)    //when finished at 0.f, play
		Transition->Play();    //update and finished
	else    //when finished at n.f, reverse
		Transition->Reverse();    //update and finished
}
