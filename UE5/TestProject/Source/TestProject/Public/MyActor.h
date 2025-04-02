// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyCharacter.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"

UCLASS()
class TESTPROJECT_API AMyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* ������� */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MySceneComponent")
	class USceneComponent* MyScene;

	/* ��̬������� */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MySceneComponent")
	class UStaticMeshComponent* MyStaticMesh;

	/* ����ϵͳ��� */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MySceneComponent")
	class UParticleSystemComponent* MyParticleSystem;

	/* ������ײ��� */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MySceneComponent")
	class UBoxComponent* MyBox;

	/* ��Ƶ��� */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MySceneComponent")
	class UAudioComponent* MyAudio;

	/* ��̬����� */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MyClass")
	TSubclassOf<AActor> MyActor;

	UFUNCTION()
	void BeginOverlapFunction(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void EndOverlapFunction(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex); 

	UFUNCTION()
	void HitFunction(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
