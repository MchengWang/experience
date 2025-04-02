// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyObject.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "MyGameInstance.h"
#include "GameFramework/Pawn.h"
#include "MyPawn.generated.h"

UENUM(BlueprintType)
namespace MyEnumType
{
	enum MyCustomEnum
	{
		Type1,
		Type2,
		Type3
	};
}

USTRUCT(BlueprintType)
struct FMyTestStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyTestStruct")
	int32 Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyTestStruct")
	FString MyName;
};

UCLASS()
class TESTPROJECT_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ������Ĭ�����ÿɼ�
	/*UPROPERTY(VisibleDefaultsOnly)
	int32 VisibleDefaultsOnly;*/

	// ����ʵ����ϸ�����ɼ�
	/*UPROPERTY(VisibleInstanceOnly)
	FString VisibleInstancestring;*/

	// ��Ĭ�����ú�ʵ����ϸ�����ɼ�
	/*UPROPERTY(VisibleAnywhere)
	FVector VisibleAnywhereVector;*/

	// ������Ĭ�����ÿɼ�
	/*UPROPERTY(EditDefaultsOnly)
	int32 EditDefaultsOnlyInt;*/

	// ����ʵ����ϸ�����ɼ�
	/*UPROPERTY(EditInstanceOnly)
	FString EditInstanceOnlystring;*/

	// ��Ĭ�����ú�ʵ����ϸ�����ɼ�
	/*UPROPERTY(EditAnywhere)
	FVector EditAnywhereVector;*/

	// ������ͼ�пɶ�
	/*UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 BlueprintReadOnlyInt;*/

	// ����ͼ�пɶ�д
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BlueprintReadWriteInt;*/

	// ����Ŀ¼
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyIntValue")
	int32 Value1;*/

	// ����Ŀ¼
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyIntValue | MySubIntValue")
	int32 Value2;*/

	// meta Ԫ����˵����
	// DisplayName ����
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "MyValur3DisplayName"))
	int32 MyValue3;*/

	// EditCondition �������Ʊ༭
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Controller"))
	bool isController;*/

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "isController"))
	float value3;*/

	// Tooltip����˵�����ǵı���
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "isControllerTrue"))
	bool isTrue;*/

	// ��¶���ǵĺ�������ͼ�е���
	/*UFUNCTION(BlueprintCallable, Category = "MyFunction")
	void PrintF1();*/

	// ���麯���Ķ��� BlueprintPure
	/*UFUNCTION(BlueprintCallable, BlueprintPure, Category = "MyFunction")
	bool PrintF2();*/

	//// BlurprintImplementablEevent �� C++ ����������ͼ������
	//UFUNCTION(BlueprintImplementableEvent)
	//void Test1();

	//UFUNCTION(BlueprintImplementableEvent)
	//int Test2();

	//UFUNCTION(BlueprintImplementableEvent)
	//void Test11(const FString& MyString);

	//UFUNCTION(BlueprintImplementableEvent)
	//int Test22(const FString& MyString);

	// BlueprintNativeEvent �� c++ ���������壬��ͼ�������ػ�����
	/*UFUNCTION(BlueprintNativeEvent)
	void TestA();

	UFUNCTION(BlueprintNativeEvent)
	int TestB();

	UFUNCTION(BlueprintNativeEvent)
	void TestC(const FString& MyString);

	UFUNCTION(BlueprintNativeEvent)
	int TestD(const FString& MyString);*/

	// Ԫ����˵���� Meta
	/*UFUNCTION(BlueprintCallable, Category = "MyFunction", meta = (DisPlayName = "MyPrinttest"))
	void Printtest();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyEnum")
	TEnumAsByte<MyEnumType::MyCustomEnum> MyCustomEnum;*/

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyCustomStruct")
	FMyTestStruct MyCustomObject;*/

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyEnum")
	TEnumAsByte<MyEnumType::MyCustomEnum> MyCustomEnum2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyTestExposeOnSpawnValue", meta = (ExposeOnSpawn = "true"))
	float ExposeOnSpawnValue;*/

	UPROPERTY()
	UMyObject* MyTestObject;

	UPROPERTY()
	UMyGameInstance* MyGameInstance;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MySceneComponent")
	USceneComponent* MyRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MySceneComponent")
	USpringArmComponent* MySpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MySceneComponent")
	UCameraComponent* MyCamera;

	UFUNCTION()
	/* ��껬���ƶ���ͷ���� */
	void Zoom(bool Direction, float ZoomSpeed);
};
