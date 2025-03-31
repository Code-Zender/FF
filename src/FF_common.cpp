#include <FF_include.h>


std::string decryptVulkanError(VkResult errorCode)
{

    switch (errorCode)
    {
    case VK_SUCCESS:
        return "Success";
        break;
    case VK_NOT_READY:
        return "A fence or query has not yet been finished";
        break;
    case VK_TIMEOUT:
        return "A result is not ready and has not yet been processed";
        break;
    case VK_EVENT_SET:
        return "An event is signaled";
        break;
    case VK_EVENT_RESET:
        return "An event is unsignaled";
        break;
    case VK_INCOMPLETE:
        return "A return array is incomplete";
        break;
    case VK_ERROR_OUT_OF_HOST_MEMORY:
        return "Not enough memory";
        break;
    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
        return "Not enough memory";
        break;
    case VK_ERROR_INITIALIZATION_FAILED:
        return "Initialization of an object has failed";
        break;
    case VK_ERROR_DEVICE_LOST:
        return "The logical or physical device has been lost";
        break;
    case VK_ERROR_MEMORY_MAP_FAILED:
        return "Mapping of a memory object has failed";
        break;
    case VK_ERROR_LAYER_NOT_PRESENT:
        return "A requested layer is not present";
        break;
    case VK_ERROR_EXTENSION_NOT_PRESENT:
        return "A requested extension is not supported";
        break;
    case VK_ERROR_FEATURE_NOT_PRESENT:
        return "A requested feature is not supported";
        break;
    case VK_ERROR_INCOMPATIBLE_DRIVER:
        return "Unable to find a Vulkan driver";
        break;
    case VK_ERROR_TOO_MANY_OBJECTS:
        return "Too many objects of the type have already been created";
        break;
    case VK_ERROR_FORMAT_NOT_SUPPORTED:
        return "A requested format is not supported on this device";
        break;
    case VK_ERROR_FRAGMENTED_POOL:
        return "A pool allocation has failed due to fragmentation";
        break;
    case VK_ERROR_UNKNOWN:
        return "An unknown error has occurred";
        break;
    case VK_ERROR_OUT_OF_POOL_MEMORY:
        return "Not enough memory";
        break;
    case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
        return "An opaque capture address is invalid";
        break;
    case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
        return "A full-screen exclusive application has lost full-screen exclusive mode";
        break;
    default:
        return "Unknown error code" + std::to_string(errorCode);
        break;
    }
}

void SDL_error(std::string message)
{
    std::cout << message << SDL_GetError() << std::endl;
}

void Vlk_error(std::string message)
{
    std::cout << "\033[31m" << message << "\033[0m" << std::endl; //TODO: ansi parsing needs to be enabled for windows
}

void ASTL_ERROR(std::string message){
    std::cout << "\033[31m" << message << "\033[0m" << std::endl;
}