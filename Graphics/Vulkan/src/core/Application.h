#pragma once

/* ���� GLFW ���� */
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Device.h"
#include "GraphicsPipeline.h"
#include "Drawing.h"
#include "VertexBuffer.h"
#include "Texture.h"

namespace WMC{

	/* Application �����ṹ */
	struct WindowsProps
	{
		const int Width;
		const int Height;
		const char* Title;

		WindowsProps(const int width = 1280, const int height = 650, const char* title = "Vulkan")
			: Width(width), Height(height), Title(title) { }
	};

	void drawFrame();

	class Application
	{
	public:
		Application();
		~Application() = default;

		/* Vulkan ���г������ */
		void run();

	private:
		/* ��ʼ�� Vulkan */
		void initVulkan();

		/* ѭ��ִ�� */
		void mainLoop();

		/* ��β����,������Դ */
		void cleanup();

	private:
		GLFWwindow* v_Window;

		WindowsProps props{};
		PhysicalDevice* v_PDevice;
		LogicalDevice* v_LDevice;
		GraphicsPipeline* v_GraphicsPipeline;
		Drawing* v_Drawing;
		VertexBuffer* v_VertexBuffer;
		Texture* v_Texture;

		/* ��ʾ���������С */
		bool framebufferResized = false;

		friend static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
	};

}