#include "GraphicsPipeline.h"
#include "VertexArray.h"

#include <fstream>

namespace WMC {

	///////////////////////////////
	////// �������� ///////////////
	//////////////////////////////
	
	static VkDevice s_Device;

	/**
	* ������ָ���ļ��ж�ȡ�����ֽڣ������ֽ�����洢�� std::vector
	*  ate ���ļ�ĩβ��ʼ��ȡ
	*  binary ���ļ���Ϊ�������ļ���ȡ
	*/
	static std::vector<char> readFile(const std::string& filename)
	{
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (!file.is_open())
		{
			throw std::runtime_error("failed to open file!");
		}

		// ʹ�ö�ȡλ����ȷ���ļ��Ĵ�С�����仺����
		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		// ���ؿ�ͷ��һ���Զ�ȡȫ���ļ�
		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
	}

	/* ������ɫ��ģ�� */
	VkShaderModule createShaderModule(const std::vector<char>& code) 
	{
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		VkShaderModule shaderModule;
		if (vkCreateShaderModule(s_Device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
			throw std::runtime_error("failed to create shader module!");
		}

		return shaderModule;
	}

	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features, VkPhysicalDevice device)
	{
		for (VkFormat format : candidates)
		{
			/**
			* VkFormatProperties�ṹ���������ֶΣ�
			*  linearTilingFeatures������ƽ��֧�ֵ�����
			*  optimalTilingFeatures�����ƽ��֧�ֵ�����
			*  bufferFeatures��������֧�ֵ�����
			*
			* ����ֻ��ǰ��������صģ����Ǽ����Ǹ�ȡ���ں����� tiling ������
			*/
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(device, format, &props);

			if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
			{
				return format;
			}
			else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
			{
				return format;
			}
		}

		throw std::runtime_error("failed to find supported format!");
	}

	VkFormat findDepthFormat(VkPhysicalDevice device) {
		return findSupportedFormat(
			{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
			VK_IMAGE_TILING_OPTIMAL,
			VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT, device
		);
	}
	
	GraphicsPipeline::GraphicsPipeline(LogicalDevice device)
		: v_LDevice(device.GetLogicalDevice())
	{
		s_Device = device.GetLogicalDevice();
		v_SwapChainImageFormat = device.GetSwapChainImageFormat();
		v_SwapChainImageViews = device.GetSwapChainImageViews();
		v_SwapChainExtent = device.GetSwapChainExtent();
		v_PDevice = device.GetPhysicalDevice();
		v_SwapChain = device.GetSwapChain();
		v_GraphicsQueue = device.GetGraphicsQueue();
		v_PresentQueue = device.GetPresentQueue();
		v_Device = &device;
	}

	void GraphicsPipeline::createGraphicsPipeline()
	{
		// ������ɫ�����ֽ���
		auto vertShaderCode = readFile("assets/shaders/texture_vert.spv");
		auto fragShaderCode = readFile("assets/shaders/texture_frag.spv");

		VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
		VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

		// ��д������ɫ���ṹ
		VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
		vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;

		// ���� Vulkan �����Ǹ��ܵ��׶���ʹ����ɫ��
		vertShaderStageInfo.module = vertShaderModule;
		vertShaderStageInfo.pName = "main";

		// ��дƬ����ɫ���ṹ
		VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
		fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;

		// ���� Vulkan �����Ǹ��ܵ��׶���ʹ����ɫ��
		fragShaderStageInfo.module = fragShaderModule;
		fragShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

		/**
		* ��������
		*  VkPipelineVertexInputStateCreateInfo ���������ݸ���ɫ���Ķ������ݵĸ�ʽ
		*  Bindings ����֮��ļ���Լ������ǰ����㻹��ʵ��
		*  Attribute descriptions ���ݸ�������ɫ�����������ͣ����Ǹ��󶨼��������Լ����Ǹ�ƫ�ƴ���������
		*
		*  pVertexBindingDescriptions �� pVertexAttributeDescriptions ��Աִ��һ���ṹ���飬
		*  ��Щ�ṹ�����������ڼ��ض������ݵ���ϸ��Ϣ
		*/
		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

		auto bindingDescription = VertexArray::getBindingDescription();
		auto attributeDescriptions = VertexArray::getAttributeDescriptions();

		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

		/**
		* �������
		*  VkPipelineInputAssemblyStateCreateInfo �����������£��Ӷ������ʲô���ļ����壬�Լ��Ƿ�Ӧ�����û�Ԫ����
		*  ���Ƽ��������ʹ������ֵ��
		*   VK_PRIMITIVE_TOPOLOGY_POINT_LIST ���Զ���ĵ�
		*   VK_PRIMITIVE_TOPOLOGY_LINE_LIST ÿ2��������ߣ�������
		*   VK_PRIMITIVE_TOPOLOGY_LINE_STRIP ÿ���ߵĽ���������������һ���ߵ���ʼ����
		*   VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST ÿ3������������Σ�������
		*   VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP ÿ�������εĵڶ����͵�����������������һ�������ε�ǰ��������
		*  ͨ���������ǰ�˳�������Ӷ��㻺�������صģ���ʹ��Ԫ�ػ�����������ָ��ʹ�õ�������������ִ���Ż������� ���ö��㡣
		*  ����� primitiveRestartEnable ��Ա����Ϊ VK_TRUE ���������_STRIP ����ģʽ��ʹ�� 0xFFFF �� 0x FFFF FFFF ������������
		*  ���������������
		*/
		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.scissorCount = 1;

		/** 
		* ��դ�� VkPipelineRasterizationStateCreateInfo
		*  �Ӷ�����ɫ���л�ȡ�ɶ�������ļ���ͼ�Σ�������ת��ΪƬ�Σ��Ա���Ƭ����ɫ����ɫ����֮����Ȳ��ԣ��沿�޳��ͼ������ԣ�
		*  ����������Ϊ��������������λ������Ե��Ƭ�Σ��߿���Ⱦ��
		*/
		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		// �������Ϊ VK_TRUE �򳬳���ƽ���ԭƽ���Ƭ�ν����̶��������Ƕ���
		rasterizer.depthClampEnable = VK_FALSE;  // ʹ�ô˹�����Ҫ���� GPU ����
		// �������Ϊ VK_TRUE �򼸺�ͼ����Զ����ͨ����դ���׶Σ�������Ͻ�����framebuffer���κ����
		rasterizer.rasterizerDiscardEnable = VK_FALSE; 
		/**
		* polygonMode ȷ������������Ƭ��
		*  VK_POLYGON_MODE_FILL ��Ƭ��������ε�����
		*  VK_POLYGON_MODE_LINE ����α�Ե����Ϊ����
		*  VK_POLYGON_MODE_POINT ����ζ������Ϊ��
		* 
		* ʹ�����������κ�ģʽ����Ҫ���� GPU ����
		*/
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		// lineWidth ��Ա�ܼ򵥣�������Ƭ�������������Ĵ�ϸ��֧�ֵ�����߿�ȡ����Ӳ�����κδ��� 1.0f ���߶���Ҫ������ wideLines GPU ���ܡ�
		rasterizer.lineWidth = 1.0f;
		// cullMode ����ȷ��Ҫʹ�õ����޳����͡������Խ����޳����޳������/���޳����档
		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		// frontFace ����ָ��Ҫ����Ϊ�������Ķ���˳�򣬿�����˳ʱ�����ʱ�롣
		rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		// ��դ������ͨ����ӳ���ֵ�����Ƭ�ε�б��ƫ�����ֵ���������ֵ������ʱ������Ӱӳ�� false ������
		rasterizer.depthBiasEnable = VK_FALSE;
		rasterizer.depthBiasConstantFactor = 0.0f; // Optional
		rasterizer.depthBiasClamp = 0.0f; // Optional
		rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

		/**
		* ���ز��� VkPipelineMultisampleStateCreateInfo ����ݵķ���֮һ
		*  ����ԭ���ǽ�դ��Ϊͬһ���صĶ������ε�Ƭ����ɫ����������һ�� ����Ҫ�����ڱ�Ե��
		*  ��Ҳ�������Եľ��α����ֵĵط��� ��Ϊ���ֻ��һ�������ӳ�䵽һ�����أ�����Ҫ�������Ƭ����ɫ����
		*  �����򵥵���Ⱦ�����߷ֱ���Ȼ����С������ȣ����ĳɱ�Ҫ�͵öࡣ ��������Ҫ���� GPU ����
		*  
		*/
		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = v_MsaaSamples;

		/**
		* ��ȸ������ڿ���ʹ���ˣ�������Ҫ��ͼ�ιܵ���������Ȳ��ԡ���ͨ�� VkPipelineDepthStencilStateCreateInfo �ṹ�������ã�
		* 
		* depthTestEnable �ֶ�ָ���Ƿ�Ӧ����Ƭ�ε��������Ȼ��������бȽϣ��Բ鿴�Ƿ�Ӧ�������ǡ�
		* �� depthWriteEnable �ֶ�ָ���Ƿ�Ӧ��ͨ����Ȳ��Ե�Ƭ�ε������ʵ��д����Ȼ�������
		*/
		VkPipelineDepthStencilStateCreateInfo depthStencil{};
		depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencil.depthTestEnable = VK_TRUE;
		depthStencil.depthWriteEnable = VK_TRUE;

		// depthCompareOp �ֶ�ָ��Ϊ��������Ƭ�ζ�ִ�еıȽϡ����Ǽ�ֽϵ͵���� = ������Լ���������Ƭ�ε����Ӧ�ø�С��
		depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;

		/*
		* depthBoundsTestEnable��minDepthBounds �� maxDepthBounds �ֶ����ڿ�ѡ����ȱ߽���ԡ�
		* �����ϣ���������ֻ����λ��ָ����ȷ�Χ�ڵ�Ƭ�Ρ����ǲ���ʹ�ô˹��ܡ�
		*/
		depthStencil.depthBoundsTestEnable = VK_FALSE;
		depthStencil.minDepthBounds = 0.0f; // Optional
		depthStencil.maxDepthBounds = 1.0f; // Optional
		
		/**
		* ��������ֶ�����ģ�建�������������̳���Ҳ����ʹ�á�
		* ���Ҫʹ����Щ�����������ȷ�����/ģ��ͼ��ĸ�ʽ����ģ�������
		*/
		depthStencil.stencilTestEnable = VK_FALSE;
		depthStencil.front = {}; // Optional
		depthStencil.back = {}; // Optional

		multisampling.minSampleShading = 1.0f; // Optional
		multisampling.pSampleMask = nullptr; // Optional
		multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
		multisampling.alphaToOneEnable = VK_FALSE; // Optional

		/**
		* ��ɫ���
		*  Ƭ����ɫ��������ɫ����Ҫ������ framebuffer �����е���ɫ�����һ������ת����Ϊ��ɫ��ϣ������ַ�������������һ��
		*   ��Ͼ�ֵ����ֵ������������ɫ
		*   ʹ�ð�λ������Ͼ�ֵ����ֵ
		*  ���������͵Ľṹ�������������ɫ��ϣ�
		*   VkPipelineColorBlendAttachmentState ����ÿ�����ӵ�֡������������
		*   VkPipelineColorBlendStateCreateInfo ����ȫ����ɫ�������
		*/
		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
											  VK_COLOR_COMPONENT_G_BIT |
											  VK_COLOR_COMPONENT_B_BIT |
											  VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE;

		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

		/**
		* ���Ҫʹ�õڶ��ֻ�Ϸ�������λ��ϣ�����Ӧ�� logicOpEnable ����Ϊ VK_TRUE��
		* Ȼ������� logicOp �ֶ���ָ����λ���㡣��ע�⣬�⽫�Զ����õ�һ�ַ�����
		* ����Ϊÿ�����ӵ�֡�������� blendEnable ����Ϊ VK_FALSEһ����
		* colorWriteMask Ҳ���ڴ�ģʽ������ȷ�� framebuffer �е���Щͨ��ʵ�ʻ��ܵ�Ӱ�졣
		* Ҳ���Խ���������ģʽ����������������������������������£�Ƭ����ɫ�������޸ĵ�д��֡��������
		*/
		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f; // Optional
		colorBlending.blendConstants[1] = 0.0f; // Optional
		colorBlending.blendConstants[2] = 0.0f; // Optional
		colorBlending.blendConstants[3] = 0.0f; // Optional

		/**
		* ��̬״̬
		*  ��Ȼ������ܵ�״̬��Ҫ�決���ܵ�״̬�У���ʵ���Ͽ��Ը�������������״̬���������ڻ���ʱ���´����ܵ���
		*  ���磬�����Ĵ�С���߿�ͻ�ϳ��������ʹ�ö�̬״̬��������Щ���ԣ��������д VkPipelineDynamicStateCreateInfo �ṹ
		*/
		std::vector<VkDynamicState> dynamicStates = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

		VkPipelineDynamicStateCreateInfo dynamicState{};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
		dynamicState.pDynamicStates = dynamicStates.data();

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 1; // Optional
		pipelineLayoutInfo.pSetLayouts = &v_DescriptSetLayout; // Optional
		pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
		pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

		if (vkCreatePipelineLayout(v_LDevice, &pipelineLayoutInfo, nullptr, &v_PipelineLayout) != VK_SUCCESS) 
		{
			throw std::runtime_error("failed to create pipeline layout!");
		}

		/**
		* ���ڽ����нṹ�Ͷ��������һ�𣬴���ͼ�ιܵ�
		*  ��ɫ���׶Σ�����ͼ�ιܵ��Ŀɱ�̽׶ι��ܵ���ɫ��ģ��
		*  �̶�����״̬�� ����ܵ��Ĺ̶����ܽ׶ε����нṹ����������ϼ�����դ������������ɫ���
		*  �ܵ����֣���ɫ�����õ� uniform �� push ֵ�������ڻ���ʱ����
		*  ��Ⱦͨ�����ܵ��׶����õĸ��������÷�
		*/
		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;

		// ���ù̶����ܽ׶ε����нṹ
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pDepthStencilState = &depthStencil; // Optional
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = &dynamicState;

	    // ���ùܵ�����
		pipelineInfo.layout = v_PipelineLayout;

		// ��ö���Ⱦͨ�������úͽ�ʹ�ô�ͼ�ιܵ�����ͨ��������
		pipelineInfo.renderPass = v_RenderPass;
		pipelineInfo.subpass = 0;

		/**
		* ʵ���ϻ�������������basePipelineHandle �� basePipelineIndex��Vulkan ������ͨ�������йܵ������������µ�ͼ�ιܵ���
		* �ܵ�������������ǣ����ܵ������йܵ�������๲ͬ����ʱ�����ùܵ��ĳɱ��ϵͣ����һ����Ը����������ͬһ�����Ĺܵ�֮���л���
		* ������ʹ�� basePipelineHandle ָ�����йܵ��ľ����Ҳ����ʹ�� basePipelineIndex ���ü�����������������һ���ܵ���
		* ����ֻ��һ���ܵ����������ֻ��ָ��һ�� null �����һ����Ч������
		* ���� VkGraphicsPipelineCreateInfo �� flags �ֶ���Ҳָ���� VK_PIPELINE_CREATE_DERIVATIVE_BIT ��־ʱ����ʹ����Щֵ��
		*/
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
		//pipelineInfo.basePipelineIndex = -1; // Optional

		if (vkCreateGraphicsPipelines(v_LDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &v_GraphicsPipeline) != VK_SUCCESS) {
			throw std::runtime_error("failed to create graphics pipeline!");
		}

		vkDestroyShaderModule(s_Device, fragShaderModule, nullptr);
		vkDestroyShaderModule(s_Device, vertShaderModule, nullptr);
	}

	/**
	* ����ɹܵ�����֮ǰ����Ҫ���� Vulkan ��Ⱦʱ��ʹ�õ�֡��������������Ҫָ�����ж��ٸ���ɫ����Ȼ�������
	* ÿ��������ʹ�ö��ٸ��������Լ���������Ⱦ������Ӧ��δ������ݡ�������Ϣ����װ��һ�� render-pass ������
	*/
	void GraphicsPipeline::createRenderPass()
	{
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = v_SwapChainImageFormat;
		colorAttachment.samples = v_MsaaSamples;

		/**
		* loadOp �� storeOp ȷ������Ⱦ֮ǰ����Ⱦ֮����δ������е����ݡ�
		* ���� loadOp������������ѡ��
		*  VK_ATTACHMENT_LOAD_OP_LOAD ���������е���������
		*  VK_ATTACHMENT_LOAD_OP_CLEAR �ڿ�ʼʱ��ֵ���Ϊ����
		*  VK_ATTACHMENT_LOAD_OP_DONT_CARE ��������δ����
		* ���� storeOp ������ѡ��
		*  VK_ATTACHMENT_STORE_OP_STORE ��Ⱦ�����ݽ��洢���ڴ��У��Ժ���Զ�ȡ
		*  VK_ATTACHMENT_STORE_OP_DONT_CARE ��Ⱦ������֡�����������ݽ�δ����
		*/
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

		/**
		* loadOp �� storeOp ��������ɫ��������ݣ�stencilLoadOp / stencilStoreOp ������ģ�����ݡ�
		* Ӧ�ó��򲻻��ģ�建����ִ���κβ�������˼��غʹ洢�Ľ���޹ؽ�Ҫ��
		*/
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

		/**
		* Vulkan �е������֡�������ɾ����ض����ظ�ʽ�� VkImage �����ʾ�����ڴ������صĲ��ֿ��ܻ���ݳ��Զ�ͼ��ִ�еĲ������ı䡣
		* 
		* һЩ����Ĳ����ǣ�
		*  VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL��������ɫ������ͼ��
		*  VK_IMAGE_LAYOUT_PRESENT_SRC_KHR��Ҫ�ڽ������г��ֵ�ͼ��
		*  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL�������ڴ渴�Ʋ���Ŀ���ͼ��
		* 
		* initialLayout ָ��ͼ������Ⱦͨ����ʼ֮ǰ�����еĲ��֡�
		* finalLayout ָ������Ⱦͨ�����ʱ�Զ����ɵ��Ĳ��֡�
		*  �� VK_IMAGE_LAYOUT_UNDEFINED ���� initialLayout ��ζ�����ǲ�����ͼ����õ���ǰ���֡�
		*  �������ֵ��Ҫע����ǣ����ܱ�֤����ͼ������ݣ����Ⲣ����Ҫ����Ϊ������ζ����������
		*  ϣ��ͼ������Ⱦ�����ʹ�ý�����������ʾ�������ʹ�� VK_IMAGE_LAYOUT_PRESENT_SRC_KHR ��Ϊ finalLayout ��ԭ��
		*/
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		/**
		* ����ע�⵽�������ѽ� finalLayout �� VK_IMAGE_LAYOUT_PRESENT_SRC_KHR ����Ϊ VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL��
		* ������Ϊ���ز���ͼ���޷�ֱ�ӳ��֡�����������Ҫ�����ǽ���Ϊ����ӳ�񡣴�Ҫ����������Ȼ���������Ϊ���������κ�ʱ����ʾ��
		* ��ˣ�����ֻ��ҪΪ color ���һ���¸���������ν�� resolve ������
		*/
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		/**
		* ��ʽӦ�����ͼ������ͬ����һ�����ǲ����Ĵ洢������� ��storeOp������Ϊ�ڻ�����ɺ󲻻�ʹ������
		* ���������Ӳ��ִ�������Ż������� color buffer һ�������ǲ�����ǰ��� depth ���ݣ�
		* ������ǿ��Խ� VK_IMAGE_LAYOUT_UNDEFINED ���� initialLayout��
		*/
		VkAttachmentDescription depthAttachment{};
		depthAttachment.format = findDepthFormat(v_PDevice);
		depthAttachment.samples = v_MsaaSamples;
		depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentDescription colorAttachmentResolve{};
		colorAttachmentResolve.format = v_SwapChainImageFormat;
		colorAttachmentResolve.samples = VK_SAMPLE_COUNT_8_BIT;
		colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		/**
		* attachment ����ͨ������ attachment descriptions �����е�����ָ��Ҫ���õĸ�����
		* ���ǵ������ɵ��� VkAttachmentDescription ��ɣ����������Ϊ 0��
		* ����ָ��������ϣ��������ʹ�ô����õ� subpass �ڼ���еĲ��֡��� subpass ����ʱ��Vulkan ���Զ�������ת��Ϊ�˲��֡�
		* ���Ǵ���ʹ�ø�����Ϊ��ɫ������������˼�壬VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL ���ֽ�Ϊ�����ṩ������ܡ�
		*/
		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentReference depthAttachmentRef{};
		depthAttachmentRef.attachment = 1;
		depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentReference colorAttachmentResolveRef{};
		colorAttachmentResolveRef.attachment = 2;
		colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		/**
		* subpass �������������������͵ĸ�����
		*  pInputAttachments������ɫ����ȡ�ĸ���
		*  pResolveAttachments�����ڶ��ز�����ɫ�����ĸ���
		*  pDepthStencilAttachment����Ⱥ�ģ�����ݵĸ���
		*  pPreserveAttachments���� subpass ��ʹ�õ�����Ϊ�䱣�����ݵĸ���
		*/
		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;
		subpass.pDepthStencilAttachment = &depthAttachmentRef;
		subpass.pResolveAttachments = &colorAttachmentResolveRef;

		// ��ͨ���������� VkSubpassDependency �ṹ��ָ��
		VkSubpassDependency dependency{};

		/**
		* ���������ֶ�ָ��������������� subpass ������������ֵ VK_SUBPASS_EXTERNAL ��ָ��Ⱦͨ��֮ǰ��֮�����ʽ��ͨ����
		* ����ȡ���������� srcSubpass ���� dstSubpass ��ָ�������� 0 �������ǵ� subpass�����ǵ�һ��Ҳ��Ψһһ����
		* dstSubpass ����ʼ�ո��� srcSubpass���Է�ֹ������ϵͼ�г���ѭ������������һ����ͨ���� VK_SUBPASS_EXTERNAL����
		*/
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		
		/**
		* �������������ֶ�ָ��Ҫ�ȴ��Ĳ����Լ�ִ����Щ�����Ľ׶Ρ�
		* ������Ҫ�ȴ���������ɴӾ����ж�ȡ��Ȼ����ܷ������������ͨ���ȴ���ɫ��������׶α�����ʵ�֡�
		*/
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
		dependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		/**
		* Ӧ�ȴ��˲����Ĳ���λ�� color attachment �׶Σ��漰 color attachment��д�롣
		* ��Щ���ý���ֹ���ɵķ�����ֱ��������Ҫ �������� ʱ����������Ҫ��ʼ����д����ɫʱ��
		*/
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		std::array<VkAttachmentDescription, 3> attachments = { colorAttachment, depthAttachment, colorAttachmentResolve };


		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		if (vkCreateRenderPass(v_LDevice, &renderPassInfo, nullptr, &v_RenderPass) != VK_SUCCESS) 
		{
			throw std::runtime_error("failed to create render pass!");
		}
	}

}