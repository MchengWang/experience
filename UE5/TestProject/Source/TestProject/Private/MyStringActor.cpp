// Fill out your copyright notice in the Description page of Project Settings.


#include "MyStringActor.h"
#include "Kismet/KismetStringLibrary.h"

// Sets default values
AMyStringActor::AMyStringActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyStringActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyStringActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyStringActor::MyStringOperation()
{
	FString MyString = TEXT("123456789_AbcdEFG");
	// �����ַ����Ƿ����ָ���ַ�
	bool isFind1 = MyString.Contains("b", ESearchCase::IgnoreCase, ESearchDir::FromStart);
	bool isFind2 = UKismetStringLibrary::Contains(MyString, "b");
	
	// �ж��ַ����Ƿ����
	bool isFind3 = MyString.Equals("123");

	// ƴ���ַ���
	FString MyString2 = MyString.Append("OBJ");

	// �ַ����ĳ���
	int32 Num = MyString.Len();
	int32 Num2 = UKismetStringLibrary::Len(MyString);

	// �ַ����Ƿ�Ϊ��
	bool isEmpty = MyString.IsEmpty();

	// ��ָ��λ�ÿ�ʼ���ַ�������һ�����ַ���
	FString MyChildString = UKismetStringLibrary::GetSubstring(MyString, 0, 3);

	// ��ָ���ַ����в������ַ�������ʼ����ֵ
	int32 Index1 = UKismetStringLibrary::FindSubstring(MyString, "123", false, false, 0);

	// ���صڼ����ַ���ASCII
	int32 Index2 = UKismetStringLibrary::GetCharacterAsNumber(MyString, 0);

	// ����һ������
	TArray<FString> MyArrayString;
	MyArrayString = UKismetStringLibrary::GetCharacterArrayFromString(MyString);

	// ��д
	FString MyUpperString = MyString.ToUpper();

	// Сд
	FString MyLowerString = MyString.ToLower();

	// ���ַ������������Ҳ����ָ���������ַ�
	FString MyLeftpadString = MyString.LeftPad(3);
	FString MyRightpadString = MyString.RightPad(3);

	// ����ַ������Ƿ���������ַ�
	bool isNumber = MyString.IsNumeric();

	// �ַ����Ƿ��Ը������ַ�����ͷ���߽�β
	bool isStart = MyString.StartsWith("abcd", ESearchCase::IgnoreCase);
	bool isEnd = MyString.EndsWith("123", ESearchCase::IgnoreCase);

	// �ַ����滻
	UKismetStringLibrary::Replace(MyString, "a", "s", ESearchCase::IgnoreCase);

	// ��������������ұ߸������ַ���
	UKismetStringLibrary::Left(MyString, 2);
	UKismetStringLibrary::Right(MyString, 2);

	// ��������߻������ұ߸������ַ�������ĩβ�е������������ַ�
	UKismetStringLibrary::LeftChop(MyString, 1);
	UKismetStringLibrary::RightChop(MyString, 2);

	// ����ʼλ�÷��� Count ���ַ������ַ���
	UKismetStringLibrary::Mid(MyString, 2, 3);

	// �ָ���ַ��������ַ���
	FString Left;
	FString Right;
	MyString.Split("_", &Left, &Right, ESearchCase::IgnoreCase, ESearchDir::FromStart);

	// �ӷָ�����ֵ�Ԫ�ַ��л�ȡ�ַ������飬���ҿ�ѡɾ�����ַ���
	TArray<FString> MyStringArrayPrase;
	MyString.ParseIntoArray(MyStringArrayPrase, TEXT("_"), true);
	for (auto TestArray : MyStringArrayPrase)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s"), *TestArray));
	}
}
