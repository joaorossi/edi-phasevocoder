#pragma once

#include "BaseProcessor.h"
#include "GenericParameterEditor.h"

class LadderFilterEditor final : public juce::AudioProcessorEditor
{
public:
    LadderFilterEditor(mrta::BaseProcessor&);
    ~LadderFilterEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    mrta::BaseProcessor& pluginProcessor;

    mrta::GenericParameterEditor paramEditor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LadderFilterEditor)
};
