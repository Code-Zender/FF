#ifndef ASTL_INCLUDE_H
#define ASTL_INCLUDE_H
#pragma once
#include <vulkan/vk_layer.h>
#include <glm/glm.hpp>
#include <stdexcept>
#include <SDL3/SDL.h>
#include <vulkan/vulkan.h>
#include <SDL3/SDL_vulkan.h>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>
#include <fstream>
#include <iostream>
#include "stb_image.h"
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>
#include <SDL3/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <string>

enum class ASTL_Result {
    ASTL_UNKNOWN = -1,
    ASTL_SUCCESS,
    ASTL_FAILED
};



// Definiere den Operator außerhalb des Enums
inline constexpr bool operator!(ASTL_Result result) noexcept {
    return result != ASTL_Result::ASTL_SUCCESS;
}

inline constexpr bool operator==(ASTL_Result result, ASTL_Result other) noexcept {
    return static_cast<int>(result) == static_cast<int>(other);
}

inline constexpr bool operator!=(ASTL_Result result, ASTL_Result other) noexcept {
    return !(result == other);
}

std::string decryptVulkanError(VkResult errorCode);
void SDL_error(std::string message);
void Vlk_error(std::string message);
void ASTL_ERROR(std::string message);

#endif