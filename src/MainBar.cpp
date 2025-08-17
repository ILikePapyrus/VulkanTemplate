#include "MainBar.h"

void showMainBar(bool* p_open)
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Profiler"))
        {
            if (ImGui::MenuItem("CPU")) {}
            if (ImGui::MenuItem("GPU")) {}
            if (ImGui::MenuItem("Battery")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("System Details")) {}
            ImGui::EndMenu();
        }
        ImGui::MenuItem("ImPlot Demo");
        ImGui::MenuItem("ImGui Demo");
        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("Shortcuts")) {}
            if (ImGui::MenuItem("About")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}