#include "MainBar.h"

#include "SoundBuffer.h"
#include "SoundDevice.h"
#include "SoundSource.h"
#include <thread>

// Boolean to keep track of open windows
bool showImPlotDemo = false;
bool showImGuiDemo = false;
bool showAudioDemo = false;
bool showAboutWindow = false;

void playSound()
{
    // Initialize audio
    SoundDevice* soundDevice;
    SoundSource speaker;
    soundDevice = SoundDevice::get();

    ALuint sound = SoundBuffer::get()->addSoundEffect("C:\\Users\\User.DESKTOP-F8V8KPI\\CLionProjects\\VulkanTemplate\\assets\\audio\\guitar-loop.wav");
    speaker.Play(sound);
}

// Draw windows
void drawAboutWindow()
{
    // Set window parameters
    ImGui::SetNextWindowPos({200, 150}, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize({400, 300}, ImGuiCond_FirstUseEver);

    // Start drawing window
    ImGui::Begin("About", &showAboutWindow);

    ImGui::Text("Vulkan Template v1.0a");
    ImGui::Text("Author: ");
    ImGui::SameLine();
    ImGui::TextLinkOpenURL("Pipirus", "https://github.com/ILikePapyrus");
    ImGui::Text("Dear ImGui %s", IMGUI_VERSION);

    // End drawing window
    ImGui::End();
};

// Draw audio player demo window
void drawAudioDemo()
{
    // Set window parameters
    ImGui::SetNextWindowPos({100, 150}, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize({600, 300}, ImGuiCond_FirstUseEver);

    // Start drawing window
    ImGui::Begin("Audio Demo", &showAudioDemo);

    // Widgets
    if (ImGui::Button("Play"))
    {
        std::thread(playSound).detach();
    }
    ImGui::SameLine();
    if (ImGui::Button("Pause"))
    {
        printf("Trying to pause audio playback, but the button doesn't have the functionality\n");
    }

    float startValue = 1;
    if (ImGui::SliderFloat("Gain", &startValue, 0, 10));

    // End drawing window
    ImGui::End();
}

void showMainBar(bool* p_open)
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Profiler"))
        {
            if (ImGui::MenuItem("CPU"))
            {
                printf("CPU Profiler\n");
            }
            if (ImGui::MenuItem("GPU"))
            {
                printf("GPU Profiler\n");
            }
            if (ImGui::MenuItem("Memory"))
            {
                printf("Memory Profiler\n");
            }
            if (ImGui::MenuItem("Battery"))
            {
                printf("Battery Profiler\n");
            }
            ImGui::Separator();
            if (ImGui::MenuItem("System Details"))
            {
                printf("System Details\n");
            }
            ImGui::EndMenu();
        }
        if (ImGui::MenuItem("ImPlot Demo", NULL, &showImPlotDemo))
        {
            printf("ImPlot Demo\n");
        };
        if (ImGui::MenuItem("ImGui Demo", NULL, &showImGuiDemo))
        {
            printf("ImGui Demo\n");
        };
        if (ImGui::MenuItem("Audio Demo", NULL, &showAudioDemo))
        {
            printf("Audio Demo\n");
            drawAudioDemo();
        }
        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("About", NULL, &showAboutWindow))
            {
                printf("About\n");
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    // Call draw windows functions when booleans change state
    if (showImPlotDemo)
    {
        ImGui::CreateContext();
        ImPlot::CreateContext();
        ImPlot::ShowDemoWindow(&showImPlotDemo);
    }

    if (showImGuiDemo)
    {
        ImGui::ShowDemoWindow();
    }

    if (showAudioDemo)
    {
        drawAudioDemo();
    }

    if (showAboutWindow)
    {
        drawAboutWindow();
    }
}