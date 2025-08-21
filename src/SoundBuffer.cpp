#include "SoundBuffer.h"
#include <sndfile.h>
#include <cinttypes>
#include <alext.h>
#include <cstdlib>

SoundBuffer* SoundBuffer::get()
{
    static SoundBuffer* soundBuffer = new SoundBuffer();

    return soundBuffer;
}

ALuint SoundBuffer::addSoundEffect(const char* path)
{
    ALenum err, format;
    ALuint buffer;
    SNDFILE* sndfile;
    SF_INFO sfinfo;
    short* membuf;
    sf_count_t num_frames;
    ALsizei num_bytes;

    sndfile = sf_open(path, SFM_READ, &sfinfo);
    if (!sndfile)
    {
        fprintf(stderr, "Could not open audio in %s: %s\n", sf_strerror(sndfile));
        return 0;
    }
    if (sfinfo.frames < 1 || sfinfo.frames >(sf_count_t)(INT_MAX / sizeof(short)) / sfinfo.channels)
    {
        fprintf(stderr, "Bad sample count in %s (%" PRId64 ")\n", path, sfinfo.frames);
        sf_close(sndfile);
        return 0;
    }

    // Select audio channel configuration
    format = AL_NONE;
    if (sfinfo.channels == 1)
    {
        format = AL_FORMAT_MONO16;
    }
    else if (sfinfo.channels == 2)
    {
        format = AL_FORMAT_STEREO16;
    }
    else if (sfinfo.channels == 3)
    {
        if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
        {
            format = AL_FORMAT_BFORMAT2D_16;
        }
    }
    else if (sfinfo.channels == 4)
    {
        if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
        {
            format = AL_FORMAT_BFORMAT3D_16;
        }
    }
    if (!format)
    {
        fprintf(stderr, "[OpenAL] Unsupported channel count: %d\n", sfinfo.channels);
        sf_close(sndfile);
        return 0;
    }

    // Decode audio file to a buffer
    membuf = static_cast<short*>(malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short)));

    num_frames = sf_readf_short(sndfile, membuf, sfinfo.frames);
    if (num_frames < 1)
    {
        free(membuf);
        sf_close(sndfile);
        fprintf(stderr, "[OpenAL] Failed to read samples in %s (%" PRId64 ")\n", path, num_frames);
        return 0;
    }
    num_bytes = (ALsizei)(num_frames * sfinfo.channels) * (ALsizei)sizeof(short);

    // Feed audio data in buffer, then free data and close file
    buffer = 0;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, format, membuf, num_bytes, sfinfo.samplerate);

    free(membuf);
    sf_close(sndfile);

    // Check for errors
    err = alGetError();
    if (err != AL_NO_ERROR)
    {
        fprintf(stderr, "[OpenAL] Error: %s\n", alGetString(err));
        if (buffer && alIsBuffer(buffer))
        {
            alDeleteBuffers(1, &buffer);
        }
        return 0;
    }

    // Add buffer to list of known buffers
    p_SoundEffectBuffers.push_back(buffer);

    return buffer;
}

// Unload buffer from memory
bool SoundBuffer::removeSoundEffect(const ALuint& buffer)
{
    auto it = p_SoundEffectBuffers.begin();

    while (it != p_SoundEffectBuffers.end())
    {
        if (*it == buffer)
        {
            alDeleteBuffers(1, &*it);

            it = p_SoundEffectBuffers.erase(it);

            return true;
        }
        else
        {
            ++it;
        }
    }

    return false;
}

SoundBuffer::SoundBuffer()
{
    p_SoundEffectBuffers.clear();
}

SoundBuffer::~SoundBuffer()
{
    alDeleteBuffers(p_SoundEffectBuffers.size(), p_SoundEffectBuffers.data());
    p_SoundEffectBuffers.clear();
}