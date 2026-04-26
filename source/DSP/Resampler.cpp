#include "Resampler.h"

#include <cmath>

Resampler::Resampler()
{
}

Resampler::~Resampler()
{
}

void Resampler::reset()
{
}

void Resampler::process(float* output, size_t outputLength, const float* input, size_t inputLength)
{
    const auto incr { static_cast<float>(inputLength) / static_cast<float>(outputLength) };
    for (size_t n = 0; n < outputLength; ++n)
    {
        const auto inputIdx { static_cast<size_t>(std::floor(n * incr)) };
        output[n] = input[inputIdx];
    }
}
