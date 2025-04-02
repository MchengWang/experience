// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "FileSystemBPLibrary.generated.h"

UENUM(BlueprintType)
enum class EFileDirectoryType : uint8
{
	Relative UMETA(DisplayName = "Relative Directory"),
	Absolute UMETA(DisplayName = "Absolute Directory")
};

UENUM(BlueprintType)
enum class EEncodingOptionsType : uint8
{
	AutoDetect,
	ForceAnsi,
	ForceUnicode,
	ForceUTF8,
	ForceUTF8WithoutBOM
};

UCLASS()
class UFileSystemBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Execute Sample function", Keywords = "FileSystem sample test testing"), Category = "FileSystemTesting")
	static float FileSystemSampleFunction(float Param);

	UFUNCTION(BlueprintCallable, Category = "File")
	static FString GetCleanDirectory(EFileDirectoryType directoryType, FString filePath);

	UFUNCTION(BlueprintCallable, Category = "File")
	static bool FileisExists(EFileDirectoryType directoryType, FString filePath);

	UFUNCTION(BlueprintCallable, Category = "File")
	static bool DirectoryIsExists(EFileDirectoryType directoryType, FString filePath);

	UFUNCTION(BlueprintCallable, Category = "File")
	static bool DeleteFile(EFileDirectoryType directoryType, FString filePath);

	UFUNCTION(BlueprintCallable, Category = "File")
	static bool DeleteDirectory(EFileDirectoryType directoryType, FString filePath);

	UFUNCTION(BlueprintCallable, Category = "File")
	static TArray<uint8> ReadBytesFromFile(EFileDirectoryType directoryType, FString filePath);

	UFUNCTION(BlueprintCallable, Category = "File")
	static void ReadFStringFromFile(EFileDirectoryType directoryType, FString filePath, FString& fileData, bool& IsSuccess);

	UFUNCTION(BlueprintCallable, Category = "File")
	static void WriteBytesToFile(EFileDirectoryType directoryType, FString filePath, TArray<uint8>& dataBytes, bool& IsSuccess);

	UFUNCTION(BlueprintCallable, Category = "File")
	static void WriteFStringToFile(EFileDirectoryType directoryType, FString filePath, FString fileData, bool& IsSuccess, EEncodingOptionsType fileEncoding);

	// �� FString ���� Base64 ����
	UFUNCTION(BlueprintPure, Category = "CommandLibrary")
	static FString StringToBase64Encode(const FString& source);

	// ���ֽڽ��� Base64 ����
	UFUNCTION(BlueprintPure, Category = "CommandLibrary")
	static FString BytesToBase64Encode(const TArray<uint8>& source);

	// �� Base64 �������Ϊ String
	UFUNCTION(BlueprintPure, Category = "CommandLibrary")
	static bool Base64DecodeFString(const FString& source, FString& dest);

	// �� Base64 �������Ϊ �ֽ�����
	UFUNCTION(BlueprintPure, Category = "CommandLibrary")
	static bool Base64DecodeBytes(const FString& source, TArray<uint8>& dest);

	// URL ����
	UFUNCTION(BlueprintPure, Category = "CommandLibrary")
	static FString URLEncode(const FString& source);

	// URL ����
	UFUNCTION(BlueprintPure, Category = "CommandLibrary")
	static FString URLDecode(const FString& source);

	// ��ȡ UTC ʱ���
	UFUNCTION(BlueprintPure, Category = "CommandLibrary")
	static FString GetUTCTimestamp();

	// ��ȡ UTC ����ʱ���
	UFUNCTION(BlueprintPure, Category = "CommandLibrary")
	static FString GetMillUTCTimestamp();

};
