#include "PhaseVocoderEditor.h"

// Width of the whole GUI
static constexpr int WIDTH { 250 };

// Height of each paramter knob on the paramEditor
static const int PARAM_HEIGHT { 100 };

PhaseVocoderEditor::PhaseVocoderEditor(PhaseVocoderProcessor& p) :
    juce::AudioProcessorEditor(p),
    pluginProcessor { p },
    paramEditor(pluginProcessor.getParamMngr(), PARAM_HEIGHT)
{
    addAndMakeVisible(paramEditor);

    setSize(WIDTH, pluginProcessor.getParamMngr().getParameters().size() * PARAM_HEIGHT);
}

PhaseVocoderEditor::~PhaseVocoderEditor()
{
}

void PhaseVocoderEditor::paint(juce::Graphics&)
{
}

void PhaseVocoderEditor::resized()
{
    paramEditor.setBounds(getLocalBounds());
}
