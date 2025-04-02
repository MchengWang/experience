#pragma once

#include "Orange/Core/Base.h"
#include "Orange/Core/Log.h"
#include <filesystem>

#ifdef OG_ENABLE_ASSERTS

	// ���ߣ����ǿ��Զ� ��WITH_MSG�� �� ��NO_MSG�� ʹ����ͬ�� ��default�� ��Ϣ������
	// ͨ�����Ӹ�ʽ�ַ��������ǽ���ʽ������Ĭ����Ϣ�У�Ϊ�Զ����ʽ�ṩ֧��
	#define OG_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { OG##type##ERROR(msg, __VA_ARGS__); OG_DEBUGBREAK(); } }
	#define OG_INTERNAL_ASSERT_WITH_MSG(type, check, ...) OG_INTERNAL_ASSERT_IMPL(type, check, "����ʧ��: {0}", __VA_ARGS__)
	#define OG_INTERNAL_ASSERT_NO_MSG(type, check) OG_INTERNAL_ASSERT_IMPL(type, check, "���� '{0}' �� {1}��{2} ʧ��", OG_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)
	#define OG_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define OG_INTERNAL_ASSERT_GET_MACRO(...) OG_EXPAND_MACRO( OG_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, OG_INTERNAL_ASSERT_WITH_MSG, OG_INTERNAL_ASSERT_NO_MSG) )
	// ��ǰ���ٽ��� condition ��һ����ѡ�ĸ��Ӳ�������Ϣ��
	#define OG_ASSERT(...) OG_EXPAND_MACRO( OG_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
	#define OG_CORE_ASSERT(...) OG_EXPAND_MACRO( OG_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
	#define OG_ASSERT(...)
	#define OG_CORE_ASSERT(...)
#endif
