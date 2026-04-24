#pragma once

#include <juce_dsp/juce_dsp.h>

class Phase
{
public:
    Phase(size_t windowSize);
    ~Phase();

    void processFrame(float* outputFrame, const float* inputFrame);

private:
    size_t windowSize;
    juce::dsp::FFT fft;

    std::vector<float> fftBuffer;

    Phase() = delete;
    Phase(const Phase&) = delete;
    Phase(Phase&&) = delete;
    const Phase& operator=(const Phase&) = delete;
    const Phase& operator=(Phase&&) = delete;
};
