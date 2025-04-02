#include "VertexArray.h"

namespace WMC {

	/* ������ */
	VkVertexInputBindingDescription VertexArray::getBindingDescription()
	{
		VkVertexInputBindingDescription bindingDescription{};

		/**
		* �������е�ÿ���������ݶ������һ�������У��������ֻ��һ���󶨡�
		*  binding ����ָ���������а󶨵�������
		*  stride ����ָ����һ����Ŀ����һ����Ŀ���ֽ�����
		*  inputRate �������Ծ�������ֵ֮һ��
		*    VK_VERTEX_INPUT_RATE_VERTEX���ƶ���ÿ������֮�����һ��������Ŀ
		*	 VK_VERTEX_INPUT_RATE_INSTANCE����ÿ��ʵ��֮���ƶ�����һ��������Ŀ
		*/
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	/* �������� */
	std::array<VkVertexInputAttributeDescription, 3> VertexArray::getAttributeDescriptions()
	{
		std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

		/**
		* binding �������� Vulkan ÿ���������ݵİ������ĸ���
		*  location �������ö�����ɫ���������λ��ָ�λ��Ϊ 0 �Ķ�����ɫ���е�������λ�ã�
		*  �������� 32 λ���������
		*   float:VK_FORMAT_R32_SFLOAT
		*   vec2:VK_FORMAT_R32G32_SFLOAT
		*   vec3:VK_FORMAT_R32G32B32_SFLOAT
		*   vec4:VK_FORMAT_R32G32B32A32_SFLOAT
		*/
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;

		/**
		* ������������Ӧ��ʹ����ɫͨ�������� shader ���������е��������ƥ��ĸ�ʽ��
		* ����ʹ�ñ���ɫ���е�������������ͨ���������ǽ�����Ĭ���������ͨ����С��Ԫ������
		* �� BGA Ԫ����ʹ��Ĭ��ֵ ��0�� 0�� 1������ɫ���� ��SFLOAT��UINT��SINT�� ��λ��ҲӦ����ɫ�����������ƥ�䡣
		* 
		* ivec2��һ�� 32 λ�з��������� 2 ��������VK_FORMAT_R32G32_SINT
		* uvec4��һ�� 32 λ�޷��������� 4 ��������VK_FORMAT_R32G32B32A32_UINT
		* double��˫���ȣ�64 λ��������VK_FORMAT_R64_SFLOAT
		*/
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

		return attributeDescriptions;
	}

}