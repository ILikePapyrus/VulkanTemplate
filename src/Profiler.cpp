#include "Profiler.h"

// Get data from Main Loop
float Profiler::getFPS() const { return currentFPS; }

float Profiler::getBatteryLevel()
{
    SYSTEM_POWER_STATUS status;
    if (GetSystemPowerStatus(&status))
    {
        return static_cast<float>(status.BatteryLifePercent);
    }
    return -1.0f;
}

Profiler::RAMStats Profiler::getRAMStats()
{
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    if (GlobalMemoryStatusEx(&memInfo))
    {
        DWORDLONG totalPhys = memInfo.ullTotalPhys;
        DWORDLONG usedPhys = totalPhys - memInfo.ullAvailPhys;

        float usedMB = static_cast<float>(usedPhys / (1024 * 1024));
        float percentUsed = static_cast<float>(usedPhys * 100.0 / totalPhys);

        return { usedMB, percentUsed };
    }

    return { -1.0f, -1.0f };
}

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

// Battery logic
void Profiler::batteryLogic()
{
    float now = static_cast<float>(glfwGetTime());
    if (batteryStartTime == 0.0f)
        batteryStartTime = now;

    float batteryLevel = Profiler::getBatteryLevel();
    if (batteryLevel >= 0.0f)
    {
        batteryHistory.push_back(batteryLevel);

        // Trigger alert
        if (batteryLevel < batteryAlertThreshold && !batteryAlertTriggered)
        {
            ImGui::OpenPopup("Battery Warning!");
            batteryAlertTriggered = true;
        }
    }

    batteryHistory.push_back(batteryLevel);
    if (batteryHistory.size() > 100) {
        batteryHistory.erase(batteryHistory.begin());
    }
}

// Memory Logic
void Profiler::memoryLogic()
{
    float now = static_cast<float>(glfwGetTime());
    if (ramStartTime == 0.0f)
        ramStartTime = now;

    RAMStats stats = getRAMStats();
    if (stats.usedMB >= 0.0f)
    {
        ramSamples.emplace_back(now - ramStartTime, stats);
    }
}

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

// Draw an ImGui + ImPlot window for Battery Usage
void Profiler::drawBatteryLevelwindow() const
{
    ImGui::Begin("Battery Usage");

    // Alert popup
    if (ImGui::BeginPopupModal("Battery Warning", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Battery below %.1f%%", batteryAlertThreshold);
        if (ImGui::Button("OK"))
            ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }

    // Plot battery % over time
    ImPlot::CreateContext();
    if (ImPlot::BeginPlot("Battery Level Over Time"))
    {
        ImPlot::PlotLine("Battery %", batteryHistory.data(), static_cast<int>(batteryHistory.size()));
        ImPlot::EndPlot();
    }

    if (!batteryHistory.empty())
    {
        ImGui::Text("Current Battery Level: %.1f%%", batteryHistory.back());
    }
    ImGui::End();
}

// Draw an ImGui + ImPlot window for Memory Usage
void Profiler::drawMemoryWindow() const
{
    ImGui::Begin("RAM Usage");

    ImPlot::CreateContext();
    if (ImPlot::BeginPlot("RAM Over Time"))
    {
        std::vector<float> times, ramValues;
        for (const auto& sample : ramSamples)
        {
            times.push_back(sample.first / 60.0f);
            ramValues.push_back(sample.second.usedMB);
        }

        ImPlot::SetupAxes("Time (min)", "RAM (MB)", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
        ImPlot::PlotLine("RAM Usage", times.data(), ramValues.data(), (int)times.size());
        ImPlot::EndPlot();
    }

    if (!ramSamples.empty())
    {
        const RAMStats& latest = ramSamples.back().second;
        ImGui::Text("Current RAM Usage: %.1f MB (%.1f%%)", latest.usedMB, latest.percentUsed);
    }

    ImGui::End();
}