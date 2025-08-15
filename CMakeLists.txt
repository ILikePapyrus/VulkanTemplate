cmake_minimum_required(VERSION 4.0)
project(VulkanTemplate)

include(FetchContent)
# Declaring libs to import
# GLFW
FETCHCONTENT_DECLARE(GLFW GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG 3.4)
FETCHCONTENT_MAKEAVAILABLE(GLFW)

# Vulkan import
find_package(Vulkan REQUIRED)
if (Vulkan_FOUND)
    include_directories(${Vulkan_INCLUDE_DIRS})
    link_directories(${Vulkan_LIBRARY_DIRS})
endif ()

set(CMAKE_CXX_STANDARD 20)

add_executable(VulkanTemplate main.cpp)

target_link_libraries(VulkanTemplate glfw ${Vulkan_LIBRARIES})