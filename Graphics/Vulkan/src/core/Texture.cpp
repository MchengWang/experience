#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace WMC {

	VertexBuffer* s_VertexBuffer = nullptr;

	extern uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDevice);

	Texture::Texture(VertexBuffer* vertexBuffer)
		: v_VertexBuffer(vertexBuffer)
	{
		s_VertexBuffer = vertexBuffer;
	}

	void Texture::createImage(uint32_t width, uint32_t height, uint32_t miplevels, VkSampleCountFlagBits numSamples, VkFormat format,
			VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory)
	{
		/**
		* �� imageType �ֶ���ָ����ͼ�����͸��� Vulkan ͼ���е����ؽ�������������ϵ����Ѱַ��
		* ���Դ��� 1D��2D �� 3D ͼ�����磬һάͼ������ڴ洢��������򽥱䣬��άͼ����Ҫ����������άͼ������ڴ洢���������
		* extent �ֶ�ָ��ͼ��ĳߴ磬������ÿ�������ж��ٸ����ء������Ϊʲô depth ����Ϊ 1 ������ 0��
		* ���ǵ���������һ�����飬�������ڲ���ʹ�� mipmapping��
		*/
		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = static_cast<uint32_t>(width);
		imageInfo.extent.height = static_cast<uint32_t>(height);
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = miplevels;
		imageInfo.arrayLayers = 1;

		// Vulkan ֧�������ܵ�ͼ���ʽ��������Ӧ�ö�����ʹ���뻺������������ͬ�ĸ�ʽ�������Ʋ�����ʧ��
		imageInfo.format = format;

		/**
		* tiling ���ֶο��Ծ�������ֵ֮һ��
		*  VK_IMAGE_TILING_LINEAR�����������ǵ�����һ����������˳������pixels
		*  VK_IMAGE_TILING_OPTIMAL�����ذ�ʵ�ֶ����˳�򲼾֣���ʵ����ѷ���
		* 
		* ��ͼ��Ĳ��ֲ�ͬ��ƽ��ģʽ�Ժ��޷����ġ����ϣ���ܹ�ֱ�ӷ���ͼ���ڴ��е����أ������ʹ�� VK_IMAGE_TILING_LINEAR��
		* ���ǽ�ʹ���ݴ滺�����������ݴ�ӳ������ⲻ�Ǳ���ġ����ǽ�ʹ�� VK_IMAGE_TILING_OPTIMAL ����ɫ�����и�Ч���ʡ�
		*/
		imageInfo.tiling = tiling;

		/**
		* ͼ��� initialLayout ֻ���������ܵ�ֵ��
		*  VK_IMAGE_LAYOUT_UNDEFINED��GPU �����ã���һ�ι��ɽ��������ء�
		*  VK_IMAGE_LAYOUT_PREINITIALIZED��GPU �����ã�����һ�����ɽ���������
		* 
		* �ڵ�һ��ת���ڼ���Ҫ�������ص�������١����ǣ����磬���Ҫ��ӳ�������ݴ�ӳ�񣬲��� VK_IMAGE_TILING_LINEAR ���ֽ��ʹ�á�
		* ����������£�����Ҫ�����������ϴ�����ͼ��Ȼ��ͼ��ת��Ϊ����Դ�������ᶪʧ���ݡ�
		* ���ǣ������ǵ�ʾ���У��������Ƚ�ͼ��ת��Ϊ����Ŀ�꣬Ȼ���������ݴӻ����������Ƶ���ͼ��
		* ������ǲ���Ҫ�����ԣ����԰�ȫ��ʹ��VK_IMAGE_LAYOUT_UNDEFINED��
		*/
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		/**
		* usage �ֶ��뻺���������ڼ�� usage �ֶξ�����ͬ�����塣��ͼ������������������Ŀ�꣬���Ӧ��������Ϊ����Ŀ�ꡣ
		* ���ǻ�ϣ���ܹ�����ɫ������ͼ����Ϊ������ɫ������÷�Ӧ���� VK_IMAGE_USAGE_SAMPLED_BIT��
		*/
		imageInfo.usage = usage;
		imageInfo.samples = numSamples;

		// ��ӳ�񽫽���һ������ϵ��ʹ�ã�֧��ͼ�Σ����Ҳ֧�֣���������Ķ���ϵ��.
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		/**
		* samples ��־����ز�����ء�����뽫����������ͼ����أ��������ʹ��һ��ʾ����������ϡ��ͼ����ص�ͼ����һЩ��ѡ��־��
		* ϡ��ͼ����ֻ��ĳЩ����ʵ�������ڴ�֧�ֵ�ͼ�����磬�����Ϊ���ص���ʹ�� 3D ���������ʹ��������������ڴ����洢���� ��air�� ֵ��
		* �ڱ��̳��У����ǲ���ʹ����������뽫�䱣��ΪĬ��ֵ 0��
		*/
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.flags = 0; // Optional

		/**
		* ��ͼ����ʹ�� vkCreateImage �����ģ���û���κ��ر�ֵ��ע��Ĳ�����ͼ��Ӳ�����ܲ�֧�� VK_FORMAT_R8G8B8A8_SRGB ��ʽ��
		* ��Ӧ����һ���ɽ��ܵ���������б���ѡ����֧�ֵ����������������ǣ��������ض���ʽ��֧�ַǳ��㷺��������ǽ������˲��衣
		* ʹ�ò�ͬ�ĸ�ʽҲ��Ҫ���˵�ת�������ǽ��� depth buffer �½��лص�������⣬���������ǽ�ʵ������һ��ϵͳ��
		*/
		if (vkCreateImage(v_LDevice, &imageInfo, nullptr, &image) != VK_SUCCESS) 
		{
			throw std::runtime_error("failed to create image!");
		}

		/**
		* Ϊͼ������ڴ�Ĺ�����ʽ��Ϊ�����������ڴ�ķ�ʽ��ȫ��ͬ��
		* ʹ�� vkGetImageMemoryRequirements ������ vkGetBufferMemoryRequirements��ʹ�� vkBindImageMemory ������ vkBindBufferMemory��
		*/
		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(v_LDevice, image, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,v_PDevice);

		if (vkAllocateMemory(v_LDevice, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate image memory!");
		}

		vkBindImageMemory(v_LDevice, image, imageMemory, 0);
	}

	void Texture::createTextureImage()
	{
		int texWidth, texHeight, texChannels;
		stbi_uc* pixels = stbi_load(path->Texture.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		VkDeviceSize imageSize = texWidth * texHeight * 4;
		v_MipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;

		if (!pixels) 
			throw std::runtime_error("failed to load texture image!");

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		v_VertexBuffer->createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(v_LDevice, stagingBufferMemory, 0, imageSize, 0, &data);
		memcpy(data, pixels, static_cast<size_t>(imageSize));
		vkUnmapMemory(v_LDevice, stagingBufferMemory);

		stbi_image_free(pixels);

		createImage(texWidth, texHeight, v_MipLevels, VK_SAMPLE_COUNT_8_BIT, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, v_TextureImage, v_TextureImageMemory);
		s_VertexBuffer->transitionImageLayout(&v_TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, v_MipLevels);
		s_VertexBuffer->copyBufferToImage(stagingBuffer, v_TextureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));

		//s_VertexBuffer->transitionImageLayout(&v_TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, v_MipLevels);
		
		SetTextureImage(v_TextureImage);

		vkDestroyBuffer(v_LDevice, stagingBuffer, nullptr);
		vkFreeMemory(v_LDevice, stagingBufferMemory, nullptr);

		v_VertexBuffer->generateMipmaps(v_TextureImage, VK_FORMAT_R8G8B8A8_SRGB, texWidth, texHeight, v_MipLevels);
	}

	VkImageView Texture::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels)
	{
		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = format;
		viewInfo.subresourceRange.aspectMask = aspectFlags;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = mipLevels;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		VkImageView imageView;
		if (vkCreateImageView(v_LDevice, &viewInfo, nullptr, &imageView) != VK_SUCCESS) 
		{
			throw std::runtime_error("failed to create image view!");
		}

		return imageView;
	}

	void Texture::createTextureImageView()
	{
		v_TextureImageView = createImageView(v_TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, v_MipLevels);
	}

	/* ������ͨ�� VkSamplerCreateInfo �ṹ�������ã��ýṹָ����ӦӦ�õ����й�������ת���� */
	void Texture::createTextureSampler()
	{
		VkPhysicalDeviceProperties properties{};
		vkGetPhysicalDeviceProperties(v_PDevice, &properties);

		/**
		* magFilter �� minFilter �ֶ�ָ����ζԷŴ����С�����ؽ��в�ֵ���Ŵ��漰�������������⣬����С�漰Ƿ������
		* ѡ����� VK_FILTER_NEAREST �� VK_FILTER_LINEAR����Ӧ����ͼ����ʾ��ģʽ��
		*/
		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;

		/**
		* ����ʹ�� addressMode �ֶ�Ϊÿ����ָ��Ѱַģʽ�������г��˿���ֵ��
		* ��ͼ��ʾ�����еĴ󲿷֡���ע�⣬��Щ���Ϊ U��V �� W�������� X��Y �� Z����������ռ������Լ����
		* 
		* VK_SAMPLER_ADDRESS_MODE_REPEAT������ͼ��ߴ�ʱ�ظ�����
		* VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT�������� repeat�����ڳ���ά��ʱ��ת�����Ծ���ͼ��
		* VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE���������ͼ��ߴ�������ı�Ե����ɫ��
		* VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE�������� clamp to edge����ʹ������� edge �෴�� edge��
		* VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER������������ͼ��ߴ�ʱ�����ش�ɫ��
		*/
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

		/**
		* �������ֶ�ָ���Ƿ�Ӧʹ�ø������Թ��ˡ�����������һ�����⣬����û�����ɲ�ʹ������maxAnisotropy �ֶ����ƿ����ڼ���������ɫ��������������
		* ֵԽ�ͣ�����Խ�ã����������Խ�͡�ҪŪ������ǿ���ʹ���ĸ�ֵ��������Ҫ���������豸�����ԣ�������ʾ��
		*/
		samplerInfo.anisotropyEnable = VK_FALSE;
		samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;

		/**
		* borderColor �ֶ�ָ����ʹ�� clamp to border Ѱַģʽ��ͼ��֮����в���ʱ����������ɫ��
		* ������ float �� int ��ʽ���� black��white �� transparent������ָ��������ɫ��
		*/
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;

		/**
		* unnormalizedCoordinates �ֶ�ָ��Ҫ���ڶ�ͼ���е����ؽ���Ѱַ������ϵ��
		* ������ֶ�VK_TRUE����ֻ��ʹ�� [0�� texWidth�� �� [0�� texHeight�� ��Χ�ڵ����ꡣ
		* ���Ϊ VK_FALSE��������������ʹ�� [0�� 1�� ��ΧѰַ���ء�ʵ��Ӧ�ó��򼸺�����ʹ�ñ�׼�����꣬
		* ��Ϊ�����Ϳ���ʹ�þ�����ȫ��ͬ����Ĳ�ͬ�ֱ��ʵ�����
		*/
		samplerInfo.unnormalizedCoordinates = VK_FALSE;

		/**
		* ��������˱ȽϹ��ܣ������Ƚ�������ֵ���бȽϣ������ñȽϵĽ������ɸѡ������
		* ����Ҫ������Ӱ��ͼ�ϵİٷֱȽӽ�ɸѡ�����ǽ�����һ����̽����һ�㡣
		*/
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.minLod = 0.0f; // Optional
		samplerInfo.maxLod = VK_LOD_CLAMP_NONE;
		samplerInfo.mipLodBias = 0.0f; // Optional

		/**
		* ��ע�⣬�������������κεط����� VkImage����������һ����ͬ�Ķ������ṩ��һ�����ڴ���������ȡ��ɫ�Ľӿڡ�
		* ������Ӧ��������Ҫ���κ�ͼ�������� 1D��2D ���� 3D���������Ͼɵ� API ��ͬ�����߽�����ͼ���ɸѡ�ϲ�Ϊ����״̬��
		*/
		if (vkCreateSampler(v_LDevice, &samplerInfo, nullptr, &v_TextureSampler) != VK_SUCCESS) 
		{
			throw std::runtime_error("failed to create texture sampler!");
		}
	}

	VkFormat Texture::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features, VkPhysicalDevice device) 
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

	VkFormat Texture::findDepthFormat(VkPhysicalDevice device) {
		return findSupportedFormat(
			{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
			VK_IMAGE_TILING_OPTIMAL,
			VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT, device
		);
	}

	bool hasStencilComponent(VkFormat format) 
	{
		return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
	}

	void Texture::createDepthResources()
	{
		VkFormat depthFormat = findDepthFormat(v_PDevice);
		createImage(v_SwapChainExtent.width, v_SwapChainExtent.height, 1, v_MsaaSamples, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, v_DepthImage, v_DepthImageMemory);
		v_DepthImageView = createImageView(v_DepthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
	}

	/**
	* OBJ �ļ���λ�á����ߡ��������������ɡ��������������Ķ�����ɣ�����ÿ������ͨ����������λ�á����ߺ�/���������ꡣ
	* �������������������������㣬���������õ������ԡ�
	*/
	void Texture::loadModel()
	{
		/**
		* attrib �������� attrib.vertices��attrib.normals �� attrib.texcoords ʸ���а�������λ�á����ߺ��������ꡣ
		* ��״�����������е����Ķ������档ÿ���涼��һ������������ɣ�ÿ�����㶼����λ�á����ߺ������������Ե�������
		* OBJ ģ�ͻ����Զ���ÿ����Ĳ��ʺ����������ǽ�������Щ��
		*/
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn, err;

		/**
		* err �ַ�����������warn �ַ������������ļ�ʱ�����ľ��棬����ȱ�ٲ��ʶ��塣
		* ���� LoadObj �������� false ʱ�����زŻ�����ʧ�ܡ�����������OBJ �ļ��е���ʵ���Ͽ��԰������������Ķ��㣬
		* �����ǵ�Ӧ�ó���ֻ����Ⱦ�����Ρ����˵��ǣ�LoadObj ��һ����ѡ�������Զ��Դ�������������ʷ֣��ò���Ĭ�ϴ�������״̬��
		*/
		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path->Obj.c_str()))
		{
			throw std::runtime_error(warn + err);
		}

		std::unordered_map<Vertex, uint32_t> uniqueVertices{};
		for (const auto& shape : shapes) {
			for (const auto& index : shape.mesh.indices) {
				Vertex vertex{};

				vertex.pos = {
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2]
				};

				vertex.texCoord = {
					attrib.texcoords[2 * index.texcoord_index + 0],
					1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
				};

				vertex.color = { 1.0f, 1.0f, 1.0f };

				if (uniqueVertices.count(vertex) == 0)
				{
					uniqueVertices[vertex] = static_cast<uint32_t>(v_Vertices.size());
					v_Vertices.push_back(vertex);
				}

				v_Indices.push_back(uniqueVertices[vertex]);
			}
		}
	}

	void Texture::createColorResources() {
		VkFormat colorFormat = v_SwapChainImageFormat;

		createImage(v_SwapChainExtent.width,v_SwapChainExtent.height, 1, v_MsaaSamples, colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, v_ColorImage, v_ColorImageMemory);
		v_ColorImageView = createImageView(v_ColorImage, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
	}

}