#pragma once

#include <vector>

class OverlapAdd
{
public:
    OverlapAdd(size_t windowSize);
    ~OverlapAdd();

    void prepare(size_t maxBufferSize);

    void writeNewFrame(const float* frameBuffer);

    size_t readyReadBuffer(float* outputBuffer, size_t length);

    void setHopSizeSamples(size_t hopSizeSamples);

private:
    std::vector<float> buffer;

    const size_t windowSize;
    size_t hopSizeSamples;
    size_t writeIndex { 0 };
    size_t readIndex { 0 };

    OverlapAdd() = delete;
    OverlapAdd(const OverlapAdd&) = delete;
    OverlapAdd(OverlapAdd&&) = delete;
    const OverlapAdd& operator=(const OverlapAdd&) = delete;
    const OverlapAdd& operator=(OverlapAdd&&) = delete;
};
