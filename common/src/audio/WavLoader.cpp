/*-----------------------------------------------------------------------

Matt Marchant 2017
http://trederia.blogspot.com

crogine - Zlib license.

This software is provided 'as-is', without any express or
implied warranty.In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions :

1. The origin of this software must not be misrepresented;
you must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment
in the product documentation would be appreciated but
is not required.

2. Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
source distribution.

-----------------------------------------------------------------------*/

#include "WavLoader.hpp"

#include <crogine/core/Log.hpp>

#include <SDL_rwops.h>

using namespace cro;
using namespace cro::Detail;

namespace
{
    const cro::uint32 riffID = 0x46464952;//"RIFF"
    const cro::uint32 formatID = 0x45564157;//"WAVE" 
    const cro::uint32 subchunk1ID = 0x20746D66;//"fmt " 
    const cro::uint32 dataID = 0x61746164; //"DATA"

    cro::uint32 asUint(const std::array<cro::int8, 4>& data)
    {
        return data[3] << 24 | data[2] << 16 | data[1] << 8 | data[0];
    }

    enum AudioFormat
    {
        PCM = 1,
        mulaw = 6,
        alaw = 7,
        IBM_MuLaw = 257,
        IBM_ALaw = 258,
        ADPCM = 259
    };
}

WavLoader::WavLoader()
    : m_dataStart   (0),
    m_dataSize      (0)
{

}

//public
bool WavLoader::open(const std::string& path)
{
    m_file.file = SDL_RWFromFile(path.c_str(), "rb");
    if (m_file.file)
    {
        //file opened, let's do stuff!
        auto read = m_file.file->read(m_file.file, &m_header, sizeof(m_header), 1);
        if (read != 1)
        {
            Logger::log("Failed to read wav header for " + path, Logger::Type::Error);
            return false;
        }

        cro::uint32 ID = asUint(m_header.chunkID);
        if (ID != riffID)
        {
            Logger::log("Header file invalid ID: " + path, Logger::Type::Error);
            return false;
        }

        ID = asUint(m_header.format);
        if (ID != formatID)
        {
            Logger::log(path + " is not a WAV format file", Logger::Type::Error);
            return false;
        }

        ID = asUint(m_header.subchunk1ID);
        if (ID != subchunk1ID)
        {
            Logger::log(path + ": Invalid header data chunk", Logger::Type::Error);
            return false;
        }

        if (m_header.audioFormat != AudioFormat::PCM)
        {
            Logger::log(path + ": not in PCM format, only PCM wav files are supported", Logger::Type::Error);
            return false;
        }

        if (m_header.bitsPerSample < 8 || m_header.bitsPerSample > 16)
        {
            Logger::log(path + ": Invalid Bits per sample, must be 8 or 16", Logger::Type::Error);
            return false;
        }

        if (m_header.channelCount > 2 || m_header.channelCount < 1)
        {
            Logger::log(path + ": invalid channel count, only mono or stereo wav files are supported", Logger::Type::Error);
            return false;
        }

        //read chunk info until we find the data and position our file stream there
        WavChunk chunk;
        read = 1;
        ID = 0;
        while (read == 1 && ID != dataID)
        {
            read = m_file.file->read(m_file.file, &chunk, sizeof(WavChunk), 1);
            ID = asUint(chunk.ID);
        }

        if (read != 1)
        {
            Logger::log("Failed to find data chunk in " + path, Logger::Type::Error);
            return false;
        }

        m_dataSize = chunk.size;
        m_dataStart = m_file.file->seek(m_file.file, sizeof(WavChunk::size), RW_SEEK_CUR);

        m_dataChunk.frequency = m_header.sampleRate;
        if (m_header.channelCount == 1)
        {
            m_dataChunk.format = (m_header.bitsPerSample == 8) ? PCMData::Format::MONO8 : PCMData::Format::MONO16;
        }
        else
        {
            m_dataChunk.format = (m_header.bitsPerSample == 8) ? PCMData::Format::STEREO8 : PCMData::Format::STEREO16;
        }

        return true;
    }
    return false; //no file :(
}

const PCMData& WavLoader::getData(std::size_t size) const
{
    auto currPos = m_file.file->seek(m_file.file, 0, RW_SEEK_CUR);

    //rewind if at the end
    if (currPos >= (m_dataStart + m_dataSize))
    {
        currPos = m_file.file->seek(m_file.file, m_dataStart, RW_SEEK_SET);
    }

    std::size_t remain = (m_dataStart + m_dataSize) - currPos;
    std::size_t byteCount = (size > 0) ? std::min(remain, size) : remain;

    if (m_sampleBuffer.size() < byteCount)
    {
        m_sampleBuffer.resize(byteCount);
    }

    m_file.file->read(m_file.file, m_sampleBuffer.data(), byteCount, 1);
    
    m_dataChunk.size = byteCount;
    m_dataChunk.data = m_sampleBuffer.data();
    return m_dataChunk;
}