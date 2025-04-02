#include "Application.h"

namespace WMC{

	//Application* application = nullptr;

	static void framebufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		auto app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
		app->framebufferResized = true;
	}

	Application::Application()
	{
		glfwInit();

		// ��ʹ�� OpenGL ����������
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		// ��ʱ���ô��ڴ�С����
		//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		
		// ����ʵ�ʴ���
		v_Window = glfwCreateWindow(props.Width, props.Height, props.Title, nullptr, nullptr);

		glfwSetWindowUserPointer(v_Window, this);
		glfwSetFramebufferSizeCallback(v_Window, [](GLFWwindow* window, int width, int height)
			{
				auto app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
				app->framebufferResized = true;
			});
	}

	void Application::run()
	{
		initVulkan();
		mainLoop();
		cleanup();
	}

	void Application::initVulkan()
	{
		v_PDevice = new PhysicalDevice(v_Window);
		
		/*  ����ʵ����ʼ�� Vulkan �� */
		v_PDevice->createInstance(); 
		
		/* ����������ʹ */
		v_PDevice->setupDebugMessenger(); 
		
		/* �������ڱ�� */
		v_PDevice->createSurface();
		
		/*  ѡ�������豸  */
		v_PDevice->pickPhysicalDevice();

		v_LDevice = new LogicalDevice(*v_PDevice);

		/* �����߼��豸 */
		v_LDevice->createLogicalDevice();
		
		/* ���������� */
		v_LDevice->createSwapChain();
		
		/*  ����ͼ����ͼ */
		v_LDevice->createImageViews();

		v_GraphicsPipeline = new GraphicsPipeline(*v_LDevice);

		/* ������Ⱦ���� */
		v_GraphicsPipeline->createRenderPass(); 

		v_Drawing = new Drawing(*v_GraphicsPipeline);
		v_VertexBuffer = new VertexBuffer();
		
		v_VertexBuffer->SetSwapChainExtent(v_LDevice->GetSwapChainExtent());
		v_VertexBuffer->SetUniformBufferMapped(v_Drawing->GetUniformBuffersMapped());
		v_VertexBuffer->SetPhysicalDevice(v_LDevice->GetPhysicalDevice());
		v_VertexBuffer->SetLogicalDevice(v_LDevice->GetLogicalDevice());

		/* �������������� */
		v_VertexBuffer->createDescriptorSetLayout();

		auto dd = v_VertexBuffer->GetLayout();
		v_GraphicsPipeline->SetDescriptorSetLayout(dd);
		v_VertexBuffer->SetDescriptorSetLayout(dd);

		/*  ����ͼ�ιܵ� */
		v_GraphicsPipeline->createGraphicsPipeline();


		auto ddes = new Drawing(*v_GraphicsPipeline);
		v_Drawing = ddes;
		v_Drawing->SetPipeline(v_GraphicsPipeline->GetPipeline());

		/* ����������� */
		v_Drawing->createCommandPool();
		
		v_VertexBuffer->SetCommandPool(v_Drawing->GetCommandPool());

		v_VertexBuffer->SetGraphicsQueue(v_LDevice->GetGraphicsQueue());

		v_Texture = new Texture(v_VertexBuffer);
		v_Texture->SetLogicalDevice(v_LDevice->GetLogicalDevice());
		v_Texture->SetPhysicalDevice(v_LDevice->GetPhysicalDevice());
		v_Texture->SetGraphicsQueue(v_LDevice->GetGraphicsQueue());

		v_Texture->SetSwapChainExtent(v_GraphicsPipeline->GetSwapChainExtent());

		v_Texture->SetSwapChainImageFormat(v_LDevice->GetSwapChainImageFormat());
		v_Texture->SetMassSamples(v_PDevice->GetMsaaSamples());

		/* ����ɫ����Դ */
		v_Texture->createColorResources();

		/* ����������� */
		v_Texture->createDepthResources();

		v_LDevice->SetDepthImage(v_Texture->GetDepthImage());
		v_LDevice->SetDepthImageView(v_Texture->GetDepthImageView());
		v_LDevice->SetDepthMemory(v_Texture->GetDepthImageMemory());

		v_Drawing->SetDepthImageView(v_Texture->GetDepthImageView());
		v_Drawing->SetColorImageView(v_Texture->GetColorImageView());

		v_GraphicsPipeline->SetMassSamples(v_PDevice->GetMsaaSamples());

		/* ����֡���� */
		v_Drawing->createFramebuffers();

		/* ��������ͼ�� */
		v_Texture->createTextureImage();

		/* ��������ͼ����ͼ */
		v_Texture->createTextureImageView();

		/* ��������� */
		v_Texture->createTextureSampler();

		v_VertexBuffer->SetTextureImageView(v_Texture->GetImageView());
		v_VertexBuffer->SetTextureSampler(v_Texture->GetImageSampler());

		/*v_Texture->SetVertices(v_VertexBuffer->GetVertices());
		v_Texture->SetIndices(v_VertexBuffer->GetIndices());*/

		/* ����ģ�� */
		v_Texture->loadModel();

		v_LDevice->SetColorImage(v_Texture->GetColorImage());
		v_LDevice->SetColorImageView(v_Texture->GetColorImageView());
		v_LDevice->SetColorMemory(v_Texture->GetColorImageMemory());

		v_VertexBuffer->SetVertices(v_Texture->GetVertices());
		v_VertexBuffer->SetIndices(v_Texture->GetIndices());

		/* �������㻺���� */
		v_VertexBuffer->createVertexBuffer();

		/* �������������� */
		v_VertexBuffer->createIndexBuffer();

		/* ����ͳһ���������� */
		v_VertexBuffer->createUniformBuffers();

		/* �������� */
		v_VertexBuffer->createDescriptorPool();

		/* �������� */
		v_VertexBuffer->createDescriptorSets();

		v_Drawing->SetDescriptorSets(v_VertexBuffer->GetDescriptorSets());

		/* ����������� */
		v_Drawing->createCommandBuffers();

		/* ����ͬ������ */
		v_Drawing->createSyncObjects();
		v_Drawing->SetUniformBuffersMapped(v_VertexBuffer->GetUniformMapped());
		v_Drawing->SetTexture(v_Texture);
		v_Drawing->SetIndexBuffer(v_VertexBuffer->GetIndexBuffer());
		v_Drawing->SetVertexBuffer(v_VertexBuffer->GetVertexBuffer());

		v_Drawing->SetDevice(v_LDevice);
	}

	void Application::mainLoop()
	{
		// ���һ���¼�ѭ��
		while (!glfwWindowShouldClose(v_Window)) {
			glfwPollEvents();
			/* ��ʼ��֡�滭 */
			v_Drawing->drawFrame(framebufferResized);
		}

		vkDeviceWaitIdle(v_LDevice->GetLogicalDevice());
	}

	void Application::cleanup()	
	{
		/* ���ú������ͷ�֡���壬ͼ����ͼ�ͽ����� */
		v_LDevice->cleanupSwapChain(v_Drawing->GetSwapChainFramebuffers());

		/* ɾ���ܵ� */
		vkDestroyPipeline(v_LDevice->GetLogicalDevice(), v_GraphicsPipeline->GetPipeline(), nullptr);

		/* ɾ���ܵ����� */
		vkDestroyPipelineLayout(v_LDevice->GetLogicalDevice(), v_GraphicsPipeline->GetPipelineLayout(), nullptr);

		/* ɾ����Ⱦ���� */
		vkDestroyRenderPass(v_LDevice->GetLogicalDevice(), v_GraphicsPipeline->GetRenderPass(), nullptr);

		/* ɾ��ͳһ���������� */
		auto Buffers = v_VertexBuffer->GetBuffers();
		auto BuffersMemory = v_VertexBuffer->GetDeviceMemory();
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) 
		{
			vkDestroyBuffer(v_LDevice->GetLogicalDevice(), Buffers[i], nullptr);
			vkFreeMemory(v_LDevice->GetLogicalDevice(), BuffersMemory[i], nullptr);
		}

		/* ɾ���������� */
		vkDestroyDescriptorPool(v_LDevice->GetLogicalDevice(), v_VertexBuffer->GetDescriptorPool(), nullptr);

		/* ɾ��������� */
		vkDestroySampler(v_LDevice->GetLogicalDevice(), v_Texture->GetImageSampler(), nullptr);

		/* ɾ��ͼ����ͼ */
		vkDestroyImageView(v_LDevice->GetLogicalDevice(), v_Texture->GetImageView(), nullptr);

		/* ɾ��ͼ�� */
		vkDestroyImage(v_LDevice->GetLogicalDevice(), v_Texture->GetTextureImage(), nullptr);

		/* �ͷ�ͼ��ռ���ڴ� */
		vkFreeMemory(v_LDevice->GetLogicalDevice(), v_Texture->GetTextureImageMemory(), nullptr);

		/* ɾ������������ */
		vkDestroyDescriptorSetLayout(v_LDevice->GetLogicalDevice(), v_VertexBuffer->GetLayout(), nullptr);

		/* ɾ������������ */
		vkDestroyBuffer(v_LDevice->GetLogicalDevice(), v_VertexBuffer->GetIndexBuffer(), nullptr);

		/* �ͷ�Ϊ����������������ڴ�ռ� */
		vkFreeMemory(v_LDevice->GetLogicalDevice(), v_VertexBuffer->GetIndexBufferMemory(), nullptr);

		/* ɾ�����㻺���� */
		vkDestroyBuffer(v_LDevice->GetLogicalDevice(), v_VertexBuffer->GetVertexBuffer(), nullptr);

		/* �ͷ�Ϊ���㻺����������ڴ�ռ� */
		vkFreeMemory(v_LDevice->GetLogicalDevice(), v_VertexBuffer->GetVertexBufferMemory(), nullptr);

		/* ɾ��ͬ���� */
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			vkDestroySemaphore(v_LDevice->GetLogicalDevice(), v_Drawing->GetImageSemaphores()[i], nullptr);
			vkDestroySemaphore(v_LDevice->GetLogicalDevice(), v_Drawing->GetRenderSemaphores()[i], nullptr);
			vkDestroyFence(v_LDevice->GetLogicalDevice(), v_Drawing->GetFences()[i], nullptr);
		}

		// ɾ���������
		vkDestroyCommandPool(v_LDevice->GetLogicalDevice(), v_Drawing->GetCommandPool(), nullptr);

		/* ʹ�ú��������豸 */
		vkDestroyDevice(v_LDevice->GetLogicalDevice(), nullptr);

		if (enableValidationLayers) {
			v_PDevice->DestroyDebugUtilsMessengerEXT(v_PDevice->GetInstance(), v_PDevice->GetDebugMessenger(), nullptr);
		}

		/* ���ٱ�������⹦�� */
		vkDestroySurfaceKHR(v_PDevice->GetInstance(), v_PDevice->GetSurface(), nullptr);

		/* ɾ��ʵ�� */
		vkDestroyInstance(v_PDevice->GetInstance(), nullptr);

		/* һ�����ڹرգ�������Ҫͨ������������ֹ GLFW ������������Դ */
		glfwDestroyWindow(v_Window);

		glfwTerminate();
	}

}