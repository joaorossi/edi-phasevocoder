#pragma once

#include <BaseProcessor.h>

#include <juce_dsp/juce_dsp.h>

#include "OverlapSave.h"
#include "OverlapAdd.h"
#include "Phase.h"
#include "Resampler.h"

class PhaseVocoderProcessor final : public BaseProcessor
{
public:
    static constexpr size_t WINDOW_SIZE { 1 << 12 };
    static constexpr size_t BASE_HOP_SIZE { WINDOW_SIZE / 8 };

    PhaseVocoderProcessor();
    ~PhaseVocoderProcessor() override;

    // Called before processing starts
    void prepare(double sampleRate, int samplesPerBlock) override;

    // Audio stream callback
    void process(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    // Creates the GUI
    juce::AudioProcessorEditor* createEditor() override;

private:
    OverlapSave olsLeft;
    OverlapSave olsRight;

    OverlapAdd olaLeft;
    OverlapAdd olaRight;

    Phase phaseLeft;
    Phase phaseRight;

    float resampleFracAcc { 0.f };

    float ratio { 1.f };
    size_t analysisHopSize { BASE_HOP_SIZE };
    size_t synthesisHopSize { BASE_HOP_SIZE };;

    juce::dsp::WindowingFunction<float> windowFunction;

    juce::LagrangeInterpolator resamplerLeft;
    juce::LagrangeInterpolator resamplerRight;

    std::vector<float> resamplerBufferLeft;
    std::vector<float> resamplerBufferRight;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PhaseVocoderProcessor)
};
