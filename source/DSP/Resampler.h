#pragma once

class Resampler
{
public:
    Resampler();
    ~Resampler();

    void reset();

    void process(float* output, size_t outputLength, const float* input, size_t inputLength);

private:

    Resampler(const Resampler&) = delete;
    Resampler(Resampler&&) = delete;
    const Resampler operator=(const Resampler&) = delete;
    const Resampler operator=(Resampler&&) = delete;
};
