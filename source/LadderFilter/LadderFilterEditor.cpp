#include "LadderFilterEditor.h"
#include "BaseProcessor.h"

// Width of the whole GUI
static constexpr int WIDTH { 250 };

// Height of each paramter knob on the paramEditor
static const int PARAM_HEIGHT { 100 };

LadderFilterEditor::LadderFilterEditor(mrta::BaseProcessor& p) :
    juce::AudioProcessorEditor(p),
    pluginProcessor { p },
    paramEditor(pluginProcessor.getParameterManager(), PARAM_HEIGHT)
{
    addAndMakeVisible(paramEditor);

    // Calculate window height based on number of parameters
    const auto height { static_cast<int>(pluginProcessor.getParameterManager().getParameters().size() * PARAM_HEIGHT) };
    setSize(WIDTH, height);
}

LadderFilterEditor::~LadderFilterEditor()
{
}

void LadderFilterEditor::paint(juce::Graphics&)
{
}

void LadderFilterEditor::resized()
{
    paramEditor.setBounds(getLocalBounds());
}
