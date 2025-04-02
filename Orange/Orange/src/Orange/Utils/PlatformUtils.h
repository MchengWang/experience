#pragma once

#include <string>
#include <optional>

namespace Orange
{

	class FileDialogs
	{
	public:
		// ���ȡ�����򷵻ؿ��ַ���
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
	};

	class Time
	{
	public:
		static float GetTime();
	};

}
