#include "VertexBuffer.h"

namespace WMC {

	/**
	* �Կ������ṩ��ͬ���͵��ڴ������䡣ÿ�����͵��ڴ�������Ĳ����������������涼������ͬ��
	* ������Ҫ����������Ҫ��������Լ���Ӧ�ó���Ҫ�������������ҵ����ʵ��ڴ�������ʹ�á�
	*/
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDevice)
	{
		/**
		* VkPhysicalDeviceMemoryProperties �ṹ���������飺memoryTypes �� memoryHeaps��
		* �ڴ���ǲ�ͬ���ڴ���Դ������ר�� VRAM �� RAM �еĽ����ռ䣬�Ա��� VRAM �ľ�ʱʹ�á�
		* ��Щ���д��ڲ�ͬ���͵��ڴ档���ڣ�����ֻ��ע�ڴ�����ͣ��������������ԵĶѣ����������������Ӱ�����ܡ�
		*/
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

		/** 
		* �ҵ��ʺϻ�����������ڴ�����
		* typeFilter ����������ָ�����ʵ��ڴ����͵�λ�ֶΡ���ζ�ſ���ͨ���򵥵ص����������Ӧ��λ�Ƿ�����Ϊ 1 ���ҵ������ڴ����͵�������
		* ���ǣ���Ӧ�ý����ʺ϶��㻺�������ڴ����͸���Ȥ������Ҫ�ܹ��Ķ�������д����ڴ档memoryTypes ������ VkMemoryType �ṹ��ɣ�
		* ��Щ�ṹָ��ÿ���ڴ����͵ĶѺ����ԡ���Щ���Զ������ڴ�����⹦�ܣ������ܹ�ӳ�������Ա����ǿ��Դ� CPU д��������������
		*/
		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
		{
			/**
			* ���ǿ����ж�� desired property���������Ӧ�ü�鰴λ AND �Ľ���Ƿ񲻽����Ƿ��㣬���ǵ��� desired properties λ�ֶΡ�
			* ��������ʺϻ��������ڴ����ͣ�������������������Ҫ���������ԣ���ô���Ƿ��������������������ǽ��׳��쳣��
			*/
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
			{
				return i;
			}
		}

		throw std::runtime_error("failed to find suitable memory type!");
	}

	void VertexBuffer::createVertexBuffer()
	{
		/* ָ���������Ĵ�С�����ֽ�Ϊ��λ�� */
		 VkDeviceSize bufferSize = sizeof(v_Vertices[0]) * v_Vertices.size();

		/**
		* ���ڣ����ǽ�ʹ�ô��� stagingBufferMemory ���� stagingBuffer ��ӳ��͸��ƶ������ݡ�
		* ��ʹ�������µĻ�����ʹ�ñ�־��
		*  VK_BUFFER_USAGE_TRANSFER_SRC_BIT���������������ڴ洫������е�Դ��
		*  VK_BUFFER_USAGE_TRANSFER_DST_BIT���������������ڴ洫������е�Ŀ�ꡣ
		*/
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
					 VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		/**
		* ���������ݸ��Ƶ���������
		* �˺����������Ƿ�����ƫ�����ʹ�С�����ָ���ڴ���Դ����������� offset �� size �ֱ�Ϊ 0 �� bufferInfo.size��
		* Ҳ����ָ������ֵ VK_WHOLE_SIZE ��ӳ�������ڴ档�����ڶ�������������ָ����־������ǰ API �������κο��õı�־��
		* ����������Ϊֵ 0�����һ������ָ��ָ�� Map �ڴ��ָ��������
		*/
		void* data;
		vkMapMemory(v_LDevice, stagingBufferMemory, 0, bufferSize, 0, &data);

		/**
		* ���ڣ���ֻ�轫�������� memcpy ��ӳ����ڴ棬Ȼ��ʹ�� vkUnmapMemory �ٴ�ȡ��ӳ�䡣
		* �ź����ǣ�����������ܲ������������ݸ��Ƶ��������ڴ��У��������ڻ��档
		* �Ի�������д��Ҳ������ Map �ڴ����в��ɼ���
		*  �����ַ������Դ�������⣺
		*  ʹ������һ�µ��ڴ�ѣ���VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		*  vkFlushMappedMemoryRanges�����ڴ�ӳ���ڴ��ȡ֮ǰ���� vkInvalidateMappedMemoryRanges
		*/
		memcpy(data, v_Vertices.data(), (size_t)bufferSize);
		vkUnmapMemory(v_LDevice, stagingBufferMemory);

		/**
		* vertexBuffer ���ڴ��豸���ص��ڴ����ͷ��䣬��ͨ����ζ�������޷�ʹ�� vkMapMemory��
		* ���ǣ����ǿ��Խ����ݴ� stagingBuffer ���Ƶ� vertexBuffer��
		* ���Ǳ���ͨ��ָ�� stagingBuffer �Ĵ���Դ��־�� vertexBuffer �Ĵ���Ŀ���־�Լ����㻺����ʹ�ñ�־���������Ǵ�����������
		*/
		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
					VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, v_VertexBuffer, v_VertexBufferMemory);
		copyBuffer(stagingBuffer, v_VertexBuffer, bufferSize);

		vkDestroyBuffer(v_LDevice, stagingBuffer, nullptr);
		vkFreeMemory(v_LDevice, stagingBufferMemory, nullptr);
	}

	void VertexBuffer::createIndexBuffer()
	{
		VkDeviceSize bufferSize = sizeof(v_Indices[0]) * v_Indices.size();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
					VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(v_LDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, v_Indices.data(), (size_t)bufferSize);
		vkUnmapMemory(v_LDevice, stagingBufferMemory);

		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
					VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, v_IndexBuffer, v_IndexBufferMemory);

		copyBuffer(stagingBuffer, v_IndexBuffer, bufferSize);

		vkDestroyBuffer(v_LDevice, stagingBuffer, nullptr);
		vkFreeMemory(v_LDevice, stagingBufferMemory, nullptr);
	}

	void VertexBuffer::createDescriptorSetLayout()
	{
		/**
		* ǰ�����ֶ�ָ����ɫ����ʹ�õİ󶨺������������ͣ���ͳһ����������
		* ��ɫ���������Ա�ʾͳһ��������������飬descriptorCount ָ�������е�ֵ����
		* ���磬�������Ϊ���������ĹǼ��е�ÿ������ָ���任��
		* ���ǵ� MVP ת��λ�ڵ���ͳһ�����������У��������ʹ�� descriptorCount 1��
		*/
		VkDescriptorSetLayoutBinding uboLayoutBinding{};
		uboLayoutBinding.binding = 0;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.descriptorCount = 1;

		/**
		* ���ǻ���Ҫָ��Ҫ���ĸ���ɫ���׶�������������
		* stageFlags �ֶο����� VkShaderStageFlagBits ֵ��ֵ VK_SHADER_STAGE_ALL_GRAPHICS ����ϡ�
		*/
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		/**
		* pImmutableSamplers �ֶν���ͼ�������ص���������أ������Ժ󽫶Դ˽��н��ܡ������Խ��䱣��ΪĬ��ֵ��
		*/
		uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

		/**
		* ȷ������ stageFlags ��ָʾ���Ǵ�����Ƭ����ɫ����ʹ����ϵ�ͼ��������������������ȷ��Ƭ����ɫ�ĵط���
		* �����ڶ�����ɫ����ʹ��������������磬ͨ���߶���ͼ��̬���ζ�������
		*/
		VkDescriptorSetLayoutBinding samplerLayoutBinding{};
		samplerLayoutBinding.binding = 1;
		samplerLayoutBinding.descriptorCount = 1;
		samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBinding.pImmutableSamplers = nullptr;
		samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };

		/**
		* Ȼ�����ǿ���ʹ�� vkCreateDescriptorSetLayout ��������
		* �������ܴ��а�����ļ� VkDescriptorSetLayoutCreateInfo��
		*/
		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		layoutInfo.pBindings = bindings.data();

		if (vkCreateDescriptorSetLayout(v_LDevice, &layoutInfo, nullptr, &v_DescriptorSetLayout) != VK_SUCCESS) 
		{
			throw std::runtime_error("failed to create descriptor set layout!");
		}
	}

	void VertexBuffer::createUniformBuffers()
	{
		VkDeviceSize bufferSize = sizeof(UniformBufferObject);

		v_UniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
		v_UniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
		v_UniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) 
		{
			createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, v_UniformBuffers[i], v_UniformBuffersMemory[i]);

			vkMapMemory(v_LDevice, v_UniformBuffersMemory[i], 0, bufferSize, 0, &v_UniformBuffersMapped[i]);
		}
	}

	void VertexBuffer::createDescriptorPool()
	{
		/**
		* ���ǻ����봴��һ��������������أ�ͨ���� VkDescriptorPoolCreateInfo �����һ�� VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER ���͵�
		* VkPoolSize ��Ϊ���ͼ��������ķ����ڳ��ռ䡣ת�� createDescriptorPool ��������������޸ģ�
		* �԰������������� VkDescriptorPoolSize��
		* 
		* �������ز�������֤���޷�����������һ���ܺõ�ʾ������ Vulkan 1.1 ��ʼ������ز�����
		* vkAllocateDescriptorSets ���ܻ�ʧ�ܲ���ʾ������� VK_ERROR_POOL_OUT_OF_MEMORY������������Ҳ���ܳ������ڲ�������⡣
		* ����ζ����ʱ ��ȡ����Ӳ�����ش�С�ͷ����С�� ��������������ǰ��ѳ��������������Ƶķ��䡣
		* ����ʱ��vkAllocateDescriptorSets ��ʧ�ܲ����� VK_ERROR_POOL_OUT_OF_MEMORY��
		* ���������ĳЩ������ϳɹ������������������ʧ�ܣ���������ر����˾�ɥ��
		*/
		std::array<VkDescriptorPoolSize, 2> poolSizes{};
		poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
		poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

		// ��Ϊÿ��֡��������һ��������
		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		// ���õĵ������������������֮��
		poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		poolInfo.pPoolSizes = poolSizes.data();

		// ָ�����Է���������������������
		poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

		if (vkCreateDescriptorPool(v_LDevice, &poolInfo, nullptr, &v_DescriptorPool) != VK_SUCCESS) 
		{
			throw std::runtime_error("failed to create descriptor pool!");
		}
	}

	void VertexBuffer::createDescriptorSets()
	{
		// ָ��Ҫ���з�����������ء�Ҫ������������������Լ����������ڵ�������������
		std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, v_DescriptorSetLayout);
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = v_DescriptorPool;
		allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
		allocInfo.pSetLayouts = layouts.data();

		v_DescriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
		if (vkAllocateDescriptorSets(v_LDevice, &allocInfo, v_DescriptorSets.data()) != VK_SUCCESS) 
		{
			throw std::runtime_error("failed to allocate descriptor sets!");
		}

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) 
		{
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = v_UniformBuffers[i];
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(UniformBufferObject);

			VkDescriptorImageInfo imageInfo{};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = v_TextureImageView;
			imageInfo.sampler = v_TextureSampler;

			std::array<VkWriteDescriptorSet, 2> descriptorWrites{};
			descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[0].dstSet = v_DescriptorSets[i];
			descriptorWrites[0].dstBinding = 0;
			descriptorWrites[0].dstArrayElement = 0;

			/**
			* ������Ҫ�ٴ�ָ�������������͡�����һ�θ��������еĶ���������������� dstArrayElement ��ʼ��
			* descriptorCount �ֶ�ָ��Ҫ���µ�����Ԫ������
			*/
			descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrites[0].descriptorCount = 1;
			descriptorWrites[0].pBufferInfo = &bufferInfo;

			descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[1].dstSet = v_DescriptorSets[i];
			descriptorWrites[1].dstBinding = 1;
			descriptorWrites[1].dstArrayElement = 0;
			descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrites[1].descriptorCount = 1;
			descriptorWrites[1].pImageInfo = &imageInfo;

			vkUpdateDescriptorSets(v_LDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		}
	}

	void VertexBuffer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
	{
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(v_LDevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
			throw std::runtime_error("failed to create buffer!");
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(v_LDevice, buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties, v_PDevice);

		if (vkAllocateMemory(v_LDevice, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) 
		{
			throw std::runtime_error("failed to allocate buffer memory!");
		}

		vkBindBufferMemory(v_LDevice, buffer, bufferMemory, 0);
	}

	void VertexBuffer::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) 
	{
		VkCommandBuffer commandBuffer = beginSingleTimeCommands();

		VkBufferCopy copyRegion{};
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

		endSingleTimeCommands(commandBuffer);
	}

	/**
	* ִ�в��ֹ��ɵ��������֮һ��ʹ��ͼ���ڴ����ϡ��������Ĺܵ�����ͨ������ͬ������Դ�ķ��ʣ�
	* ����ȷ���ڴӻ�������ȡ֮ǰ��ɶԻ�������д�룬����Ҳ��������ʹ�� VK_SHARING_MODE_EXCLUSIVEʱת��ͼ�񲼾ֺ�ת�ƶ���ϵ������Ȩ��
	* ��һ����Ч�Ļ������ڴ����Ͽ���Ϊ������ִ�д˲�����
	*/
	void VertexBuffer::transitionImageLayout(VkImage* image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels)
	{
		VkCommandBuffer commandBuffer = beginSingleTimeCommands();

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;

		// ǰ�����ֶ�ָ�����ֹ��ɡ������������ͼ����������ݣ�����Խ� VK_IMAGE_LAYOUT_UNDEFINED ���� oldLayout��
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;

		/**
		* �����ʹ��������ת�ƶ���ϵ������Ȩ����ô�������ֶ�Ӧ���Ƕ���ϵ�е�������
		* ���������������������뽫��������Ϊ VK_QUEUE_FAMILY_IGNORED������Ĭ��ֵ��
		*/
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

		// image �� subresourceRange ָ����Ӱ���ͼ���ͼ����ض����֡����ǵ�ͼ�������飬Ҳû�� mipmap �������ָֻ����һ������Ͳ㡣
		barrier.image = *image;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = mipLevels;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		/**
		* ������Ҫ����ͬ��Ŀ�ģ����������ָ���漰��Դ����Щ���͵Ĳ�������������֮ǰ�������Լ��漰��Դ����Щ���������������ϵȴ���
		* ���������Ѿ�ʹ�� vkQueueWaitIdle �������ֶ�ͬ����������Ҳ��Ҫ��������
		* ��ȷ��ֵȡ���ھɲ��ֺ��²��֣���ˣ�һ������ȷ����Ҫʹ�õĹ��ɣ����Ǿͻ�ص�������⡣
		*/
		barrier.srcAccessMask = 0; // TODO
		barrier.dstAccessMask = 0; // TODO

		/**
		* ���ϱ�����ʾ������д����뷢���ڹܵ�����׶Ρ�����д�벻�صȴ��κ����ݣ�
		* ���������Ϊ pre-barrier ����ָ��һ���յķ�������;�������� pipeline stage VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT��
		* Ӧ��ע����ǣ�VK_PIPELINE_STAGE_TRANSFER_BIT ������ͼ�κͼ���ܵ��е���ʵ�׶Ρ���������һ������ת�Ƶ�α�׶Ρ�
		* ����� �ĵ� ���˽������Ϣ��α�׶ε�����ʾ����
		* 
		* ͼ��д��ͬһ�ܵ��׶Σ������Ƭ����ɫ����ȡ�������������Ƭ����ɫ���ܵ��׶�ָ����ɫ����ȡ����Ȩ�޵�ԭ��
		* ������ǽ�����Ҫ�������ת������ô���ǽ���չ������Ӧ�ó�������Ӧ�óɹ����У���Ȼ��û���Ӿ��仯��
		* 
		* ��Ҫע���һ���ǣ���������ύ�ᵼ���ڿ�ʼʱ��ʽVK_ACCESS_HOST_WRITE_BITͬ����
		* ���� transitionImageLayout ������ʹ�õ�������ִ�������������ˣ�������ڲ��ֹ�������ҪVK_ACCESS_HOST_WRITE_BIT�����
		* �����ʹ�ô���ʽͬ������ srcAccessMask ����Ϊ 0���Ƿ�Ҫ��ȷ˵��ȡ�����������Ҹ��˲�ϲ��������Щ���� OpenGL �ġ����ء�������
		* 
		* ʵ���ϣ���һ���������͵�ͼ�񲼾�֧�����в�����VK_IMAGE_LAYOUT_GENERAL����Ȼ������������������һ��Ϊ�κβ����ṩ������ܡ�
		* ��ĳЩ��������£����罫ͼ����������������������ͼ���뿪Ԥ��ʼ�����ֺ��ȡͼ��ʱ�����Ǳ���ġ�
		* 
		* ��ĿǰΪֹ���ύ��������а�����������������Ϊͨ���ȴ����б�Ϊ������ͬ��ִ�С�
		* ����ʵ��Ӧ�ó��򣬽��齫��Щ�����ϲ���������������У����첽ִ������������������������� createTextureImage �����еĹ��ɺ͸��ơ�
		* ����ͨ�����������������������¼���� setupCommandBuffer ��������һ�㣬����� flushSetupCommands ��ִ�е�ĿǰΪֹ�Ѽ�¼�����
		* ���������ӳ�乤����ִ�д˲������Լ��������Դ�Ƿ���������ȷ��
		*/
		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) 
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) 
		{
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) 
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		}
		else
		{
			throw std::invalid_argument("unsupported layout transition!");
		}

		/*
		* �������͵Ĺܵ��ϰ���ʹ����ͬ�ĺ����ύ�����������ĵ�һ������ָ������������ barrier ֮ǰӦ�����Ĺܵ��׶Ρ�
		* �ڶ�������ָ���������� barrier �ϵȴ��Ĺܵ��׶Ρ�������������֮ǰ��֮��ָ���Ĺܵ��׶�ȡ������������֮ǰ��֮�����ʹ����Դ��
		* �����ֵ���ڹ淶�Ĵ˱��С����磬���Ҫ������֮��� uniform ��ȡ���ݣ�
		* �����ָ�� VK_ACCESS_UNIFORM_READ_BIT ���÷��Լ����� uniform �ж�ȡ��������ɫ����Ϊ�ܵ��׶Σ�
		* ���� VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT��Ϊ����ʹ��ָ������ɫ���ܵ��׶�û�����壬���ҵ���ָ����ʹ�����Ͳ�ƥ��Ĺܵ��׶�ʱ��
		* ��֤�㽫�����������档
		* 
		* ����������Ϊ 0 �� VK_DEPENDENCY_BY_REGION_BIT�����߽�����ת��Ϊÿ�������������
		* ����ζ�ţ����磬���� implementation �Ѿ���ʼ�ӵ�ĿǰΪֹ��д����Դ���ֿ�ʼ��ȡ��
		* 
		* ������Բ����������ֿ������͵Ĺܵ��������飺�ڴ����ϡ��������ڴ����Ϻ�ͼ���ڴ����ϣ���������������ʹ�õ�һ����
		* ��ע�⣬���ǻ�û��ʹ�� VkFormat �����������ǽ��ò���������Ȼ������½��е�������ɡ�
		*/
		vkCmdPipelineBarrier(
			commandBuffer,
			sourceStage, destinationStage,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);

		endSingleTimeCommands(commandBuffer);
	}


	VkCommandBuffer VertexBuffer::beginSingleTimeCommands() 
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = v_CommandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(v_LDevice, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		return commandBuffer;
	}

	void VertexBuffer::endSingleTimeCommands(VkCommandBuffer commandBuffer) {
		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(v_GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(v_GraphicsQueue);

		vkFreeCommandBuffers(v_LDevice, v_CommandPool, 1, &commandBuffer);
	}

	void VertexBuffer::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
	{
		VkCommandBuffer commandBuffer = beginSingleTimeCommands();

		/**
		* ��Щ�ֶ��еĴ�������ǲ��������ġ�bufferOffset ָ������ֵ��ʼ�Ļ������е��ֽ�ƫ������
		* bufferRowLength �� bufferImageHeight �ֶ�ָ���������ڴ��еĲ��ַ�ʽ�����磬��������ͼ�����֮����һЩ����ֽڡ�
		* Ϊ����ָ�� 0 ��ʾ����ֻ��������һ���������С�imageSubresource��imageOffset �� imageExtent �ֶ�ָʾҪ�����ظ��Ƶ�ͼ����ĸ����֡�
		*/
		VkBufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;

		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;

		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = {
			width,
			height,
			1
		};

		/**
		* ���ĸ�����ָʾͼ��ǰʹ�õĲ��֡������������ͼ���Ѿ����ɵ����ʺϽ����ظ��Ƶ��Ĳ��֡�
		* ���ڣ�����ֻ��һ�����ؿ鸴�Ƶ�����ͼ�񣬵�����ָ��һ�� VkBufferImageCopy ���飬ͨ��һ�β����Ӹû�������ͼ��ִ����಻ͬ�ĸ��ơ�
		*/
		vkCmdCopyBufferToImage(
			commandBuffer,
			buffer,
			image,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1,
			&region
		);

		endSingleTimeCommands(commandBuffer);
	}

	void VertexBuffer::generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels)
	{
		// Check if image format supports linear blitting
		VkFormatProperties formatProperties;
		vkGetPhysicalDeviceFormatProperties(v_PDevice, imageFormat, &formatProperties);

		if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) 
		{
			throw std::runtime_error("texture image format does not support linear blitting!");
		}

		VkCommandBuffer commandBuffer = beginSingleTimeCommands();

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.image = image;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;
		barrier.subresourceRange.levelCount = 1;

		int32_t mipWidth = texWidth;
		int32_t mipHeight = texHeight;

		for (uint32_t i = 1; i < mipLevels; i++)
		{
			barrier.subresourceRange.baseMipLevel = i - 1;
			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

			vkCmdPipelineBarrier(commandBuffer,
				VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
				0, nullptr,
				0, nullptr,
				1, &barrier);

			VkImageBlit blit{};
			blit.srcOffsets[0] = { 0, 0, 0 };
			blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
			blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.srcSubresource.mipLevel = i - 1;
			blit.srcSubresource.baseArrayLayer = 0;
			blit.srcSubresource.layerCount = 1;
			blit.dstOffsets[0] = { 0, 0, 0 };
			blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
			blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.dstSubresource.mipLevel = i;
			blit.dstSubresource.baseArrayLayer = 0;
			blit.dstSubresource.layerCount = 1;

			vkCmdBlitImage(commandBuffer,
				image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				1, &blit,
				VK_FILTER_LINEAR);

			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			vkCmdPipelineBarrier(commandBuffer,
				VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
				0, nullptr,
				0, nullptr,
				1, &barrier);

			if (mipWidth > 1) mipWidth /= 2;
			if (mipHeight > 1) mipHeight /= 2;
		}

		barrier.subresourceRange.baseMipLevel = mipLevels - 1;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
			0, nullptr,
			0, nullptr,
			1, &barrier);

		endSingleTimeCommands(commandBuffer);
	}

}