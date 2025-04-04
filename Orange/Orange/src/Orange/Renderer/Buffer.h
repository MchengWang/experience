#pragma once

namespace Orange
{
	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return 4;
		case ShaderDataType::Float2:   return 4 * 2;
		case ShaderDataType::Float3:   return 4 * 3;
		case ShaderDataType::Float4:   return 4 * 4;
		case ShaderDataType::Mat3:     return 4 * 3 * 3;
		case ShaderDataType::Mat4:     return 4 * 4 * 4;
		case ShaderDataType::Int:      return 4;
		case ShaderDataType::Int2:     return 4 * 2;
		case ShaderDataType::Int3:     return 4 * 3;
		case ShaderDataType::Int4:     return 4 * 4;
		case ShaderDataType::Bool:     return 1;
		}

		OG_CORE_ASSERT(false, "未知的着色器数据类型！");
		return 0;
	}

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		size_t Offset;
		bool Normalized;

		BufferElement() = default;

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false) 
			:Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
		{
		}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
			case ShaderDataType::Float:      return 1;
			case ShaderDataType::Float2:     return 2;
			case ShaderDataType::Float3:     return 3;
			case ShaderDataType::Float4:     return 4;
			case ShaderDataType::Mat3:       return 3;
			case ShaderDataType::Mat4:       return 4;
			case ShaderDataType::Int:        return 1;
			case ShaderDataType::Int2:       return 2;
			case ShaderDataType::Int3:       return 3;
			case ShaderDataType::Int4:       return 4;
			case ShaderDataType::Bool:       return 1;
			}
			OG_CORE_ASSERT(false, "未知的着色器数据类型！");
			return 0;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() {}

		BufferLayout(std::initializer_list<BufferElement> elements) 
		:o_Elements(elements)
		{
			CalculateOffsetsAndStride();
		}

		uint32_t GetStride() const { return o_Stride; }
		const std::vector<BufferElement>& GetElements() const { return o_Elements; }

		std::vector<BufferElement>::iterator begin() { return o_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return o_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return o_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return o_Elements.end(); }

	private:
		void CalculateOffsetsAndStride()
		{
			size_t offset = 0;
			o_Stride = 0;
			for (auto& element : o_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				o_Stride += element.Size;
			}
		}

	private:
		std::vector<BufferElement> o_Elements;
		uint32_t o_Stride = 0;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(const void* data, uint32_t size) = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		static Ref<VertexBuffer> Create(uint32_t size);
		static Ref<VertexBuffer> Create(float* vertices, uint32_t size);
	};

	// 当前仅支持32位索引缓冲
	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);
	};
}
