#pragma once

#include <BaseProcessor.h>

class PhaseVocoderProcessor final : public BaseProcessor
{
public:
    PhaseVocoderProcessor();
    ~PhaseVocoderProcessor() override;

    // Called before processing starts
    void prepare(double sampleRate, int samplesPerBlock) override;

    // Audio stream callback
    void process(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    // Creates the GUI
    juce::AudioProcessorEditor* createEditor() override;

private:
    float gain { 1.f };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PhaseVocoderProcessor)
};
