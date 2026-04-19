#pragma once

#include <BaseProcessor.h>

#include <juce_dsp/juce_dsp.h>

#include "OverlapSave.h"
#include "OverlapAdd.h"

class PhaseVocoderProcessor final : public BaseProcessor
{
public:
    static constexpr size_t WINDOW_SIZE { 1 << 12 };

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

    juce::dsp::WindowingFunction<float> windowFunction;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PhaseVocoderProcessor)
};
