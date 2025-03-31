#pragma once
#include <FF_include.h>

namespace Renderer{
//-----Nur um dich zu nerven-----
    struct Instance{
        VkInstance hand;
        void create();
    };
//-------------------------------
    struct Image{
        VkImage handle;
        VkImageView view;
        VkExtent2D extent;
        VkFormat format;
        VkImageLayout layout;
        VkDeviceMemory memoryHandle;
        VkDeviceSize memorySize;
        void create();
        void destroy();
    };

    struct Buffer{
        VkBuffer handle;
        VkDeviceMemory memoryHandle;
        VkDeviceSize memorySize;
        void create();
        void Destroy();
    };

    struct Device{
        VkDevice handle;
        VkPhysicalDevice phyDevice;
        VkQueue queue;
        void create(Context *ctx);
    };

    struct SwapChain{
        VkSwapchainKHR handle;
        std::vector<Image> images;
        void create(Context *ctx);
    };

    class texture{
        public:
            VkImage image = VK_NULL_HANDLE;
            VkImageView imageView = VK_NULL_HANDLE;
            VkDeviceMemory imageMemory = VK_NULL_HANDLE;
            VkDescriptorSet descriptorSet = VK_NULL_HANDLE;

            void create();
            void destroy();
    };


    class Context{
        public:
            void init();

            Instance instance;
            Device device;
            SwapChain swapChain;

            SDL_Window *window;
            VkSurfaceKHR surface;

        private:



    };
    struct Scene{
        void create();
        struct Obj{

        };
        std::vector<Obj> Objects;
    };
};