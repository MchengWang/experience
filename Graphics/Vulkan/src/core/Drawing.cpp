#include "Drawing.h"

namespace WMC {

    /* Ѱ�Һ��ʵĶ��� */
    extern QueueFamilyIndices findQueueFamilies(VkPhysicalDevice& device);

    /**
       * ���ڣ����ǽ���ʼ���� recordCommandBuffer �������ú�����������Ҫִ�е�����д�����������
       * ʹ�õ� VkCommandBuffer ����Ϊ�������룬�Լ�������Ҫд��ĵ�ǰ������ӳ���������
       */
    void Drawing::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
    {
        /* �����������Ѿ���¼��һ�Σ������ vkBeginCommandBuffer ����ʽ���������Ժ��޷�������׷�ӵ��������� */
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        
        /**
        * flags ����ָ�����ǽ����ʹ���������������ֵ���ã�
        *  VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT�������������ִ��һ�κ���������¼�ơ�
        *  VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT������һ�������������������ȫλ�ڵ�����Ⱦͨ���С�
        *  VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT��������������ڵȴ�ִ��ʱ�����ύ��
        */
        beginInfo.flags = 0; // Optional

        /* pInheritanceInfo �������븨�����������ء���ָ��Ҫ�ӵ�������������̳е�״̬��*/
        beginInfo.pInheritanceInfo = nullptr; // Optional

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        // ��������ʹ�� vkCmdBeginRenderPass ��ʼ��Ⱦͨ������Ⱦͨ��ʹ�� VkRenderPassBeginInfo �ṹ�е�һЩ�����������á�
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;

        /**
        * ��һ����������Ⱦͨ�������Ҫ�󶨵ĸ���������Ϊÿ��������ͼ�񴴽���һ��֡����������������ָ��Ϊ��ɫ������
        * ��ˣ�������ҪΪҪ���ƵĽ�����ͼ���֡��������ʹ�ô���� imageIndex ���������ǿ���Ϊ��ǰ������ͼ��ѡ����ȷ��֡��������
        */
        renderPassInfo.renderPass = v_RenderPass;
        renderPassInfo.framebuffer = v_SwapChainFramebuffers[imageIndex];

        /**
        * ����������������������Ⱦ����Ĵ�С����Ⱦ��������ɫ�����غʹ洢��λ�á�
        * ������֮������ؽ�����δ�����ֵ����Ӧ�븽���Ĵ�Сƥ�䣬�Ի��������ܡ�
        */
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = v_SwapChainExtent;

        /**
        * ������������������� VK_ATTACHMENT_LOAD_OP_CLEAR �� clear ֵ�����ǽ���������ɫ�����ļ��ز�����
        * �ѽ�͸����ɫ����Ϊ���� 100% ��͸���ȵĺ�ɫ��
        */
        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
        clearValues[1].depthStencil = { 1.0f, 0 };

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        /**
        * ÿ������ĵ�һ������ʼ���ǽ�����¼�Ƶ������������ 
        *  �ڶ�������ָ�����Ǹո��ṩ����Ⱦͨ������ϸ��Ϣ�� final ������������ṩ��Ⱦͨ���еĻ������ �����Ծ�����������ֵ֮һ��
        *  VK_SUBPASS_CONTENTS_INLINE����Ⱦͨ�����Ƕ�뵽��������������У����Ҳ���ִ�и������������
        *  VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS����Ⱦͨ������Ӹ����������ִ�С�
        */
        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, v_PipeLine);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)(v_SwapChainExtent.width);
        viewport.height = (float) (v_SwapChainExtent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = v_SwapChainExtent;
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

        /**
        * vkCmdBindVertexBuffers �������ڽ����㻺�����󶨵��󶨣�������������һ�������õ������������������֮�⣬
        * ǰ����������ָ�������ǽ�Ϊ��ָ�����㻺������ƫ�����Ͱ����������������ָ��Ҫ�󶨵Ķ��㻺�������飬
        * �Լ�Ҫ���п�ʼ��ȡ�������ݵ��ֽ�ƫ����������Ӧ���Ķ� vkCmdDraw �ĵ��ã��Դ��ݻ������еĶ�������������Ӳ��������� 3��
        */
        VkBuffer vertexBuffers[] = { v_VertexBuffer };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

        vkCmdBindIndexBuffer(commandBuffer, v_IndexBuffer, 0, VK_INDEX_TYPE_UINT32);
        
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, v_PipelineLayout, 0, 1, &v_DescriptorSets[currentFrame], 0, nullptr);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

        /**
        * ʵ�ʵ� vkCmdDraw �����е㷴����������������ǰָ����������Ϣ��������ǳ��򵥡������������֮�⣬�����������²�����
        *  vertexCount����ʹ����û�ж��㻺�������Ӽ����Ͻ���������Ȼ�� 3 ��������Ҫ���ơ�
        *  instanceCount������ʵ������Ⱦ�������������������ʹ�á�1
        *  firstVertex���������㻺������ƫ���������� ����Сֵ��gl_VertexIndex
        *  firstInstance������ʵ������Ⱦ��ƫ���������� ����Сֵ��gl_InstanceIndex
        */
        //vkCmdDraw(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

        vkCmdEndRenderPass(commandBuffer);

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
    }

	Drawing::Drawing(GraphicsPipeline& graphicsPipeline)
		: v_LDevice(graphicsPipeline.GetLogicalDevice())
	{
		v_SwapChainImageViews = graphicsPipeline.GetSwapChainImageViews();
        v_SwapChainExtent = graphicsPipeline.GetSwapChainExtent();
        v_PDevice = graphicsPipeline.GetPhysicalDevice();
        v_SwapChain = graphicsPipeline.GetSwapChain();
        v_GraphicsQueue = graphicsPipeline.GetGraphicsQueue();
        v_PresentQueue = graphicsPipeline.GetPresentQueue();
        v_RenderPass = graphicsPipeline.GetRenderPass();
        v_PipeLine = graphicsPipeline.GetPipeline();
        v_PipelineLayout = graphicsPipeline.GetPipelineLayout();
	}
	
	void Drawing::createFramebuffers()
	{
		v_SwapChainFramebuffers.resize(v_SwapChainImageViews.size());

        // ����ͼ����ͼ�����д���֡������
        for (size_t i = 0; i < v_SwapChainImageViews.size(); i++) {
            std::array<VkImageView, 3> attachments = {
               v_ColorImageView,
               v_DepthImageView,
               v_SwapChainImageViews[i]
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = v_RenderPass;

            /**
            * attachmentCount �� pAttachments ����ָ��Ӧ�󶨵���Ⱦͨ�� pAttachment ��������Ӧ���������� VkImageView ����
            */
            framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
            framebufferInfo.pAttachments = attachments.data();

            /**
            * width �� height ����һĿ��Ȼ��layers ��ָͼ�������еĲ��������ǵĽ����������ǵ��ž������Բ����� 1��
            */
            framebufferInfo.width = v_SwapChainExtent.width;
            framebufferInfo.height = v_SwapChainExtent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(v_LDevice, &framebufferInfo, nullptr, &v_SwapChainFramebuffers[i]) != VK_SUCCESS) 
            {
                throw std::runtime_error("failed to create framebuffer!");
            }
        }
	}

    void Drawing::createCommandPool()
    {
        QueueFamilyIndices queueFamilyIndices = findQueueFamilies(v_PDevice);

        VkCommandPoolCreateInfo poolInfo{};
        /**
        * ��������������ܵı�־��
        *  VK_COMMAND_POOL_CREATE_TRANSIENT_BIT����ʾ�����������ʹ������������¼�ƣ����ܻ�����ڴ������Ϊ��
        *  VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT������������¼��������������û�д˱�־�����Ƕ�����һ������
        */
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

        if (vkCreateCommandPool(v_LDevice, &poolInfo, nullptr, &v_CommandPool) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create command pool!");
        }
    }

    /**
    * �������ͨ�� vkAllocateCommandBuffers �������䣬�ú����� VkCommandBufferAllocateInfo �ṹ��Ϊ������
    * ����ָ��Ҫ���������غͻ�����������
    */
    void Drawing::createCommandBuffers()
    {
        v_CommandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = v_CommandPool;

        /**
        * level ����ָ����������������������������Ǹ������������
        *  VK_COMMAND_BUFFER_LEVEL_PRIMARY�������ύ������ִ�У������ܴ���������������á�
        *  VK_COMMAND_BUFFER_LEVEL_SECONDARY������ֱ���ύ�������Դ�������������á�
        */
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (uint32_t)v_CommandBuffers.size();

        if (vkAllocateCommandBuffers(v_LDevice, &allocInfo, v_CommandBuffers.data()) != VK_SUCCESS) 
        {
            throw std::runtime_error("failed to allocate command buffers!");
        }
    }

    void Drawing::createSyncObjects()
    {
        v_ImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        v_RenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        v_InFlightFences.resize(MAX_FRAMES_IN_FLIGHT);


        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            if (vkCreateSemaphore(v_LDevice, &semaphoreInfo, nullptr, &v_ImageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(v_LDevice, &semaphoreInfo, nullptr, &v_RenderFinishedSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(v_LDevice, &fenceInfo, nullptr, &v_InFlightFences[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to create semaphores!");
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////
    ///////// �� Vulkan ����Ⱦ֡�����Ĳ��� ///////////////////////////////////////
    ///////////// �ȴ���һ֡��� /////////////////////////////////////////////////
    ///////////// �ӽ�������ȡͼ�� ///////////////////////////////////////////////
    ///////////// ¼��һ��������������ڽ��������Ƶ���ͼ���� ////////////////////
    ///////////// �ύ¼�Ƶ�������� ///////////////////////////////////////////
    ///////////// ��ʾ������ͼ�� /////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////

    void Drawing::drawFrame(bool& framebufferResized)
    {
        /** 
        * ��֡�Ŀ�ͷ������ϣ���ȵ�ǰһ֡��ɣ��Ա�����������ź����ɹ�ʹ�á�
        * Ϊ�ˣ����ǵ��� vkWaitForFences��
        */
        vkWaitForFences(v_LDevice, 1, &v_InFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

        /**
        * vkAcquireNextImageKHR ��ǰ�����������߼��豸������ϣ�����л�ȡͼ��Ľ�������
        *  ����������ָ��ͼ����õĳ�ʱʱ�䣨������Ϊ��λ����ʹ�� 64 λ�޷������������ֵ��ζ������ʵ���Ͻ����˳�ʱ��
        *  ����������������ָ������ʾ����ʹ����ͼ��ʱҪ�����źŵ�ͬ������
        *  ��������ǿ��Կ�ʼ��������ʱ��㡣����ָ���ź�����դ�������ߡ�
        *  ���ǽ�������ʹ�����ǵ� imageAvailableSemaphore ��ʵ�����Ŀ�ġ�
        *  ���һ������ָ��һ����������������ѱ�Ϊ���õĽ�����ӳ���������
        *  ���������� swapChainImages �����е� VkImage�����ǽ�ʹ�ø�������ѡ�� VkFrameBuffer��
        */
        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(v_LDevice, v_SwapChain, UINT64_MAX, v_ImageAvailableSemaphores[currentFrame],
            VK_NULL_HANDLE, &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            recreateSwapChain();
            return;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        updateUniformBuffer(currentFrame);


        // ������Ҫͨ�������ֶ���դ������Ϊ unsignaled ״̬��
        vkResetFences(v_LDevice, 1, &v_InFlightFences[currentFrame]);

        /**
        * ʹ�� imageIndex ָ��Ҫʹ�õĽ�����ӳ���������ڿ��Լ�¼���������
        * ���ȣ���������������ϵ��� vkResetCommandBuffer����ȷ�����ܹ�����¼��
        * vkResetCommandBuffer �ĵڶ��������� VkCommandBufferResetFlagBits ��־���������ǲ������κ���������飬��˽��䱣��Ϊ 0��
        */
        vkResetCommandBuffer(v_CommandBuffers[currentFrame], 0);

        recordCommandBuffer(v_CommandBuffers[currentFrame], imageIndex);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        /**
        * ǰ��������ָ����ִ�п�ʼ֮ǰҪ�ȴ���Щ�ź������Լ��ڹܵ����ĸ��׶εȴ���
        *  ����ϣ���ȴ�����ɫд��ͼ��ֱ�������ã��������ָ����д����ɫ������ͼ�ιܵ��Ľ׶Ρ�
        *  ����ζ�Ŵ������Ͻ�����ͼ���в�����ʱ��ʵ���Ѿ����Կ�ʼִ�����ǵĶ�����ɫ���ȡ�
        *  waitStages �����е�ÿ����Ŀ����Ӧ�� pWaitSemaphores �о�����ͬ�������ź�����
        */
        VkSemaphore waitSemaphores[] = { v_ImageAvailableSemaphores[currentFrame]};
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        // ָ��Ҫʵ���ύ�Թ�ִ�е��������
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &v_CommandBuffers[currentFrame];

        /* signalSemaphoreCount �� pSignalSemaphores ����ָ��������������ִ�к�Ҫ����Щ�ź��������ź� */
        VkSemaphore signalSemaphores[] = { v_RenderFinishedSemaphores[currentFrame]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(v_GraphicsQueue, 1, &submitInfo, v_InFlightFences[currentFrame]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        /**
        * ǰ��������ָ������ʾ֮ǰҪ�ȴ���Щ�ź��������� VkSubmitInfo һ����
        * ����������ȴ�����������ִ�У���˻��������ǵ������Σ�
        * ������ǻ�ȡ��Ҫ�����źŵ��ź������ȴ����ǣ��������ʹ�� signalSemaphores��
        */
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = { v_SwapChain };

        // ��������ָ��Ҫ������ʾͼ��Ľ������Լ�ÿ����������ͼ������
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;

        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pResults = nullptr; // Optional

        /**
        * ��������ֻ��ҪŪ�����ʱ��Ҫ���´����������������������µ� recreateSwapChain ������
        * ���˵��ǣ�Vulkan ͨ����������ǣ�����ʾ�ڼ䣬�����������㹻��
        * vkAcquireNextImageKHR �� vkQueuePresentKHR �������Է�����������ֵ����ʾ��һ�㡣
        * 
        *  VK_ERROR_OUT_OF_DATE_KHR��������������治���ݣ��޷���������Ⱦ�� ͨ�������ڵ������ڴ�С֮��
        *  VK_SUBOPTIMAL_KHR���������Կ����ڳɹ����ֵ����棬���������Բ�����ȫƥ�䡣
        */
        result = vkQueuePresentKHR(v_PresentQueue, &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) 
        {
            framebufferResized = false;
            recreateSwapChain();
        }
        else if (result != VK_SUCCESS) 
        {
            throw std::runtime_error("failed to present swap chain image!");
        }

        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    void Drawing::recreateSwapChain()
    {
        int width = 0, height = 0;
        glfwGetFramebufferSize(v_Window, &width, &height);
        while (width == 0 || height == 0)
        {
            glfwGetFramebufferSize(v_Window, &width, &height);
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(v_LDevice);

        v_Device->cleanupSwapChain(v_SwapChainFramebuffers);

        v_Device->createSwapChain();
        v_Device->createImageViews();
        v_Texture->createColorResources();
        v_Texture->createDepthResources();
        createFramebuffers();
    }

    void Drawing::updateUniformBuffer(uint32_t currentImage)
    {
        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        UniformBufferObject ubo{};
        ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.proj = glm::perspective(glm::radians(45.0f), (float)v_SwapChainExtent.width / (float)v_SwapChainExtent.height, 0.1f, 10.0f);

        ubo.proj[1][1] *= -1;
        memcpy(v_UniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
    }

}