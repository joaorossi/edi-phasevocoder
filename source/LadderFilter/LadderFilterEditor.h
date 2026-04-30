#pragma once

#include "LadderFilterProcessor.h"
#include "GenericParameterEditor.h"

class LadderFilterEditor final : public juce::AudioProcessorEditor
{
public:
    LadderFilterEditor(LadderFilterProcessor&);
    ~LadderFilterEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    LadderFilterProcessor& pluginProcessor;

    mrta::GenericParameterEditor paramEditor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LadderFilterEditor)
};
