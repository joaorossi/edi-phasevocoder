#include "PhaseVocoderProcessor.h"
#include "PhaseVocoderEditor.h"

static const std::vector<mrta::ParameterInfo> params
{
    mrta::ParameterInfo("gain", "Gain", "X", 0.5f, 0.f, 1.f, 0.001f, 1.f)
};

PhaseVocoderProcessor::PhaseVocoderProcessor() :
    BaseProcessor(params)
{
    paramMngr.registerParameterCallback("gain",
        [this] (float value, bool)
        {
            gain = value;
        });
}

PhaseVocoderProcessor::~PhaseVocoderProcessor()
{
}

void PhaseVocoderProcessor::prepare(double, int)
{
}

void PhaseVocoderProcessor::process(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    buffer.applyGain(gain);
}

juce::AudioProcessorEditor* PhaseVocoderProcessor::createEditor()
{
    return new PhaseVocoderEditor(*this);
}

CREATE_PLUGIN(PhaseVocoderProcessor)
