#pragma once

#include <string>
#include "include/AL/al.h"
#include "include/AL/alc.h"

class AudioEngine
{
public:
    AudioEngine();
    ~AudioEngine();
    bool Init();
    bool LoadSound(const std::string& path);
    void Play();
    void Stop();
    void SetVolume(float gain);
    void Cleanup();

private:
    ALuint buffer;
    ALuint source;
    ALCdevice* device;
    ALCcontext* context;
    bool isInitialized;
};
