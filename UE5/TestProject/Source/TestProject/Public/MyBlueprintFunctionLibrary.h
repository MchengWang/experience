// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UMyBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	// �����ļ���Դ
	UFUNCTION(BlueprintCallable, Category = "File")
	static bool LoadStringFromFile(FString FilePath, FString& resultString);

	// д���ļ���Դ
	UFUNCTION(BlueprintCallable, Category = "File")
	static bool WriteStringToFile(TArray<FString> SaveFile, FString FilePath);

	// ��ȡ�ļ�����·��
	UFUNCTION(BlueprintCallable, Category = "File")
	static FString GetFilePath(FString Path);

	// ��ȡ�ļ������޺�׺��
	UFUNCTION(BlueprintCallable, Category = "File")
	static FString GetFileName(FString InPath, bool bRemovePath);

	// ��ȡ�ļ���׺
	UFUNCTION(BlueprintCallable, Category = "File")
	static FString GetFileExtension(FString InPath, bool bInCludeDot);

	// �����ļ���
	UFUNCTION(BlueprintCallable, Category = "File")
	static void CreateFolder(FString FolderName);

	// ɾ���ļ���
	UFUNCTION(BlueprintCallable, Category = "File")
	static void DeleteFolder(FString FolderName);

	// �ƶ��ļ���
	UFUNCTION(BlueprintCallable, Category = "File")
	static bool MoveFileTo(FString To, FString From);

	// �����ļ�Ŀ¼�����е��ļ��л����ļ�
	UFUNCTION(BlueprintCallable, Category = "File")
	static TArray<FString> FindFileFolder(FString Path, FString Filter, bool Files, bool Directory);

	UFUNCTION(BlueprintCallable, Category = "������")
	static void TestPrintf();

	UFUNCTION(BlueprintCallable, Category = "Json")
	static void ReadJson(FString JsonValueString);

	UFUNCTION(BlueprintCallable, Category = "JsonToMap")
	static TMap<FString, FString> JsonToMap(FString JsonValueString);	

	UFUNCTION(BlueprintCallable, Category = "MapToJson")
	static FString MapToJson(TMap<FString, FString> InMap);

};
