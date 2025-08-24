#include "Profiler.h"

#include "GLFW/glfw3.h"

// FPS processing logic
void Profiler::frame()
{
    // Calculate FPS
    static float lastTime = 0.0f;
    float currentTime = static_cast<float>(glfwGetTime());
    currentFPS = 1.0f / (currentTime - lastTime);
    lastTime = currentTime;

    // Store history
    fpsHistory.push_back(currentFPS);
    if (fpsHistory.size() > 100) {
        fpsHistory.erase(fpsHistory.begin());
    }
}

// Get data from Main Loop
float Profiler::getFPS() const { return currentFPS; }

// Draw an ImGui + ImPlot window for FPS
void Profiler::drawFPSwindow() const
{
    ImGui::Begin("FPS");

    ImPlot::CreateContext();
    if (ImPlot::BeginPlot("FPS Over Time"))
    {
        ImPlot::PlotLine("FPS", fpsHistory.data(), static_cast<int>(fpsHistory.size()));
        ImPlot::EndPlot();
    }

    ImGui::Text("Current FPS: %.2f", currentFPS);
    ImGui::End();
}