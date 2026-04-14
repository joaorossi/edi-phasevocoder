#pragma once

#include "PhaseVocoderProcessor.h"
#include "GenericParameterEditor.h"

class PhaseVocoderEditor final : public juce::AudioProcessorEditor
{
public:
    PhaseVocoderEditor(PhaseVocoderProcessor&);
    ~PhaseVocoderEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    PhaseVocoderProcessor& pluginProcessor;

    mrta::GenericParameterEditor paramEditor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PhaseVocoderEditor)
};
