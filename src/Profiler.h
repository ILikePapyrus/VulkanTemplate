#pragma once

// Libs
#include <chrono>
#include "imgui.h"
#include "implot.h"
#include <cstdio>
#include <vector>

class Profiler
{
public:
    void frame();
    float getFPS() const;
    void drawFPSwindow() const;
private:
    std::vector<float> fpsHistory;
    float currentFPS = 0.0f;
};

// Global declaration of profiler instance
inline Profiler profiler;