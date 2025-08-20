#include "AudioEngine.h"
#include <iostream>
#include <vector>
#include <fstream>

AudioEngine::AudioEngine : device(nullptr), context(nullptr), buffer(0), source(0), isInitialized(false) {}

AudioEngine::~AudioEngine()
{
    Cleanup();
}

bool AudioEngine::Init()
{
    device = alcOpenDevice(nullptr);
    if (!device)
    {
        std::cerr << "Failed to open audio device\n" << std::endl;
        return false;
    }

    context = alcCreateContext(device, nullptr);
    if (!context)
    {
        std::cerr << "Failed to create or set audio context\n" << std::endl;
        return false;
    }

    alGenBuffers(1, &buffer);
    alGenSources(1, &source);
    isInitialized = true;
    return true;
}

bool AudioEngine::LoadSound(const std::string& path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file)
    {
        std::cerr << "Failed to open sound file\n" << std::endl;
        return false;
    }

    file.seekg(44); // Skip WAV header
    std::vector<char> data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    alBufferData(buffer, AL_FORMAT_MONO16, data.data(), data.size(), 44100);
    alSourcei(source, AL_BUFFER, buffer);
    return true;
}

void AudioEngine::Play()
{
    alSourcePlay(source);
}

void AudioEngine::Stop()
{
    alSourceStop(source);
}

void AudioEngine::SetVolume(float gain)
{
    alSourcef(source, AL_GAIN, gain);
}

void AudioEngine::Cleanup()
{
    if (isInitialized)
    {
        alDeleteSources(1, &source);
        alDeleteBuffers(1, &buffer);
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(context);
        alcCloseDevice(device);
        isInitialized = false;
    }
}
