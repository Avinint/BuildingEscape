// copyright bruno avinint

#include "OpenDoor.h"
#include "Gameframework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

void UOpenDoor::OpenDoor()
{
	AActor* Owner = GetOwner();
	FRotator Rotation = FRotator(0.0f, OpenAngle, 0.0f);
	Owner->SetActorRotation(Rotation);
	LastDoorOpenTime = GetWorld()->GetTimeSeconds();
}

void UOpenDoor::CloseDoor()
{
	AActor* Owner = GetOwner();
	FRotator Rotation = FRotator(0.0f, 90.0f, 0.0f);
	Owner->SetActorRotation(Rotation);
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the trigger volume
	// if the Opener is in the volume
	if (TotalMassOfActorsOnPlate() > 30.f) // TODO make into a parameter
	{
		OpenDoor();
	}
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay) {
		CloseDoor();
	}
}

float UOpenDoor::TotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;
	TArray<AActor*>OverlappingActors;
	// find all the overlapping actors
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	// iterate through them adding their masses
	for (const auto* Actor : OverlappingActors)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *Actor->GetName())
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}

