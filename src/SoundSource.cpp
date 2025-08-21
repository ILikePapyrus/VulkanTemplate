#include "SoundSource.h"

#include <iostream>

// Sets default values for the source
SoundSource::SoundSource()
{
    alGenSources(1, &p_soundSource);
    alSourcef(p_soundSource, AL_PITCH, p_Pitch);
    alSourcef(p_soundSource, AL_GAIN, p_Gain);
    alSource3f(p_soundSource, AL_POSITION, p_Position[0], p_Position[1], p_Position[2]);
    alSource3f(p_soundSource, AL_VELOCITY, p_Velocity[0], p_Velocity[1], p_Velocity[2]);
    alSourcei(p_soundSource, AL_LOOPING, p_LoopSound);
    alSourcei(p_soundSource, AL_BUFFER, p_Buffer);
}

// Destructor
SoundSource::~SoundSource()
{
    alDeleteSources(1, &p_soundSource);
}

// Play the sound
void SoundSource::Play(const ALuint bufferToPlay)
{
    if (bufferToPlay != p_Buffer)
    {
        p_Buffer = bufferToPlay;
        alSourcei(p_soundSource, AL_BUFFER, (ALint)p_Buffer);
    }

    alSourcePlay(p_soundSource);

    // Block thread to play sound
    // (Unnecessary if implemented in a game loop)
    ALint state = AL_PLAYING;
    std::cout << "Playing sound!\n";
    while (state == AL_PLAYING && alGetError() == AL_NO_ERROR)
    {
        std::cout << "Currently playing sound\n";
        alGetSourcei(p_soundSource, AL_SOURCE_STATE, &state);
    }

    std::cout << "Finished playing sound!\n";
}