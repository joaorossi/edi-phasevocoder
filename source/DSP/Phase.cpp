#include "Phase.h"

template<typename T>
constexpr T Log2(T x)
{
    return x == 1 ? 0 : 1 + Log2(x >> 1);
}

Phase::Phase(size_t _windowSize) :
    windowSize(_windowSize),
    fft(static_cast<int>(Log2(windowSize))),
    fftBuffer(windowSize * 2)
{
    std::fill(fftBuffer.begin(), fftBuffer.end(), 0.f);
}

Phase::~Phase()
{
}

void Phase::processFrame(float* outputFrame, const float* inputFrame)
{
    std::memcpy(fftBuffer.data(), inputFrame, windowSize * sizeof(float));
    fft.performRealOnlyForwardTransform(fftBuffer.data(), true);
    fft.performRealOnlyInverseTransform(fftBuffer.data());
    std::memcpy(outputFrame, fftBuffer.data(), windowSize * sizeof(float));
}
