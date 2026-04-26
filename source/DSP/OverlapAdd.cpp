#include "OverlapAdd.h"

OverlapAdd::OverlapAdd(size_t _windowSize) :
    windowSize { _windowSize },
    hopSizeSamples { windowSize / 4 }
{
}

OverlapAdd::~OverlapAdd()
{
}

void OverlapAdd::prepare(size_t maxBufferSize)
{
    buffer.resize(2 * windowSize + maxBufferSize);
    std::fill(buffer.begin(), buffer.end(), 0.f);
    writeIndex = readIndex = 0;
}

void OverlapAdd::writeNewFrame(const float* frameBuffer)
{
    const auto bufferSize { buffer.size() };

    const auto writeSamples0 { std::min(bufferSize - writeIndex, windowSize) };
    const auto writeSamples1 { windowSize - writeSamples0 };

    for (size_t n = 0; n < writeSamples0; ++n)
        buffer[writeIndex + n] += frameBuffer[n];
    for (size_t n = 0; n < writeSamples1; ++n)
        buffer[n] += frameBuffer[writeSamples0 + n];

    writeIndex += hopSizeSamples;
    if (writeIndex >= bufferSize)
        writeIndex -= bufferSize;
}

size_t OverlapAdd::readyReadBuffer(float* outputBuffer, size_t length)
{
    const auto bufferSize { buffer.size() };

    auto numSamplesAvailable { (writeIndex + bufferSize) - readIndex };
    if (numSamplesAvailable >= bufferSize)
        numSamplesAvailable -= bufferSize;

    const auto samplesToRead { std::min(numSamplesAvailable, length) };
    const auto readSamples0 { std::min(bufferSize - readIndex, samplesToRead) };
    const auto readSamples1 { samplesToRead - readSamples0 };

    std::memcpy(&outputBuffer[0], &buffer[readIndex], readSamples0 * sizeof(float));
    std::memcpy(&outputBuffer[readSamples0], &buffer[0], readSamples1 * sizeof(float));

    std::memset(&buffer[readIndex], 0, readSamples0 * sizeof(float));
    std::memset(&buffer[0], 0, readSamples1 * sizeof(float));

    readIndex += samplesToRead;
    if (readIndex >= bufferSize)
        readIndex -= bufferSize;

    return samplesToRead;
}

void OverlapAdd::setHopSizeSamples(size_t newHopSizeSamples)
{
    hopSizeSamples = std::min(newHopSizeSamples, windowSize);
}
