#pragma once

// Libs
#include <chrono>
#include "imgui.h"
#include "implot.h"
#include <cstdio>
#include <vector>
#include "GLFW/glfw3.h"
#include <windows.h>

class Profiler
{
public:
    struct RAMStats
    {
        float usedMB;
        float percentUsed;
    };
    void frame();
    float getFPS() const;
    float getBatteryLevel();
    static RAMStats getRAMStats();
    void batteryLogic();
    void memoryLogic();
    void drawFPSwindow() const;
    void drawBatteryLevelwindow() const;
    void drawMemoryWindow() const;
    void drawSystemDetailsWindow() const;
private:
    std::vector<float> fpsHistory;
    std::vector<float> batteryHistory;
    float currentFPS = 0.0f;
    float batteryStartTime = 0.0f;
    float batteryAlertThreshold = 20.0f;
    bool batteryAlertTriggered = false;
    std::vector<std::pair<float, RAMStats>> ramSamples;
    float ramStartTime = 0.0f;
};

// Global declaration of profiler instance
inline Profiler profiler;