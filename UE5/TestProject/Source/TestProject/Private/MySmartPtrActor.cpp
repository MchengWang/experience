// Fill out your copyright notice in the Description page of Project Settings.


#include "MySmartPtrActor.h"

// Sets default values
AMySmartPtrActor::AMySmartPtrActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMySmartPtrActor::BeginPlay()
{
	Super::BeginPlay();
	//TestAFunction();  
	//TestBFunction();  
	TestCFunction();  
}

// Called every frame
void AMySmartPtrActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMySmartPtrActor::TestAFunction()
{
	// ͨ������C++ԭ��ָ�룬��ת����UE����ָ�� ���ַ�ʽ���׻���ԭ���͹���ָ�룬���Ƽ�
	A* ptr1 = new A(1, 3.2f);
	TSharedPtr<A> Sharedptr2(ptr1); 

	// ����ָ����ֵ��ʼ��
	TSharedPtr<A> Sharedptr3(new A(2, 3.60f));

	// ͨ�����������ʼ��
	TSharedPtr<A> Sharedptr4(Sharedptr2);

	// UE �Ƽ��Ĺ���ָ���ʼ��
	TSharedPtr<A> Sharedptr5 = nullptr; // ����Ϊ��
	Sharedptr5 = MakeShareable(new A(10284, 368.0f));

	// �̰߳�ȫ�Ĺ���
	TSharedPtr<A, ESPMode::ThreadSafe> Sharedptr6(new A(10284, 360.f));
	
	// ����ָ�볣�õĽӿ�
	if (Sharedptr5.IsValid()) // ָ���Ƿ���Ч
	{
		TSharedRef<A> SharedRef1(new A(10284, 368.f));
		SharedRef1 = Sharedptr5.ToSharedRef(); // ��ָ��ת��Ϊ���ã����ü�����1
		int32 Count1 = Sharedptr5.GetSharedReferenceCount(); // ��ȡ���ü���
		UE_LOG(LogTemp, Warning, TEXT("Count1 is %d"), Count1);
		if (!Sharedptr5.IsUnique()) // �ж��Ƿ�Ψһ�����ü���Ϊ1
		{
			UE_LOG(LogTemp, Warning, TEXT("sharedptr5 is not Unique"));
		}
		// �����þ���ԭ��ָ��
		UE_LOG(LogTemp, Warning, TEXT("a of the ptr is %d"), Sharedptr5.Get()->a);
		Sharedptr5.Reset(); // ������ָ����Ϊ��
		int32 Count2 = Sharedptr5.GetSharedReferenceCount();
		UE_LOG(LogTemp, Warning, TEXT("Count2 is %d"), Count2);
	}
}

void AMySmartPtrActor::TestBFunction()
{
	// �������ó�ʼ����ʱ�����ָ��һ����Ч�Ķ���
	TSharedRef<A> SharedRef2(new A(10284, 368.0f));
	if (SharedRef2.IsUnique()) // �ж������Ƿ�Ψһ
	{
		UE_LOG(LogTemp, Warning, TEXT("2a is %d"), SharedRef2->a);
		// ���� -> ָ��
		TSharedPtr<A> Sharedptr6;
		Sharedptr6 = SharedRef2;
		UE_LOG(LogTemp, Warning, TEXT("2a is %f"), Sharedptr6.Get()->b);
	}
}

void AMySmartPtrActor::TestCFunction()
{
	// ��ָ������ѭ�����ã�ֻ����ָ�뱣�����ò��������ü���
	// ������֯�������٣������ָ��ָ��������٣���ָ����Զ����
	TSharedPtr<A> Sharedptr7 = MakeShareable(new A(10284, 368.0f));
	TSharedPtr<A> SharedRef3(new A(10284, 368.0f));

	// ������ָ�벢��ʼ��
	TWeakPtr<A> WeakPtr1(Sharedptr7);
	TWeakPtr<A> WeakPtr2(SharedRef3);
	if (WeakPtr1.IsValid())
	{
		TSharedPtr<A> Sharedptr8(WeakPtr1.Pin());
		if (Sharedptr8.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("8a is %d"), Sharedptr8.Get()->a);
		}
	}
}

