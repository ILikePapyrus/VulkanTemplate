cmake_minimum_required(VERSION 4.0)
project(VulkanTemplate)
# Build target
add_executable(VulkanTemplate
        main.cpp
        src/Profiler.cpp
        src/Profiler.h
        src/MainBar.cpp
        src/MainBar.h
        src/SoundBuffer.cpp
        src/SoundBuffer.h
        src/SoundDevice.cpp
        src/SoundDevice.h
        src/SoundSource.cpp
        src/SoundSource.h
)

include(FetchContent)

# Declaring libs to import
# GLFW
FETCHCONTENT_DECLARE(GLFW GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG 3.4)
FETCHCONTENT_MAKEAVAILABLE(GLFW)

# OpenAL-Soft
find_package(OpenAL CONFIG REQUIRED)

# libsndfile
find_package(SndFile CONFIG REQUIRED)

# ImGUI
set(IMGUI_DIR external/imgui/)
target_include_directories(VulkanTemplate PRIVATE
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)
target_sources(VulkanTemplate PRIVATE
    ${IMGUI_DIR}imgui.cpp
    ${IMGUI_DIR}imgui_draw.cpp
    ${IMGUI_DIR}imgui_tables.cpp
    ${IMGUI_DIR}imgui_widgets.cpp
    ${IMGUI_DIR}imgui_demo.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
)

# ImPlot
set(IMPLOT_DIR external/implot/)
target_include_directories(VulkanTemplate PRIVATE
    ${IMPLOT_DIR}
)
target_sources(VulkanTemplate PRIVATE
    ${IMPLOT_DIR}implot.cpp
    ${IMPLOT_DIR}implot_items.cpp
    ${IMPLOT_DIR}implot_demo.cpp
)

# Vulkan import
find_package(Vulkan REQUIRED)
if (Vulkan_FOUND)
    include_directories(${Vulkan_INCLUDE_DIRS})
    link_directories(${Vulkan_LIBRARY_DIRS})
endif ()

set(CMAKE_CXX_STANDARD 20)

# Assets import
file(COPY ${CMAKE_SOURCE_DIR}/assets/fonts/ DESTINATION ${CMAKE_BINARY_DIR}/assets/fonts)
file(COPY ${CMAKE_SOURCE_DIR}/assets/audio/ DESTINATION ${CMAKE_BINARY_DIR}/assets/audio)

# Import libs
target_link_libraries(VulkanTemplate glfw ${Vulkan_LIBRARIES} OpenAL::OpenAL SndFile::sndfile)