#include "PhaseVocoderProcessor.h"
#include "PhaseVocoderEditor.h"

static const std::vector<mrta::ParameterInfo> params
{
    mrta::ParameterInfo("gain", "Gain", "X", 0.5f, 0.f, 1.f, 0.001f, 1.f)
};

PhaseVocoderProcessor::PhaseVocoderProcessor() :
    BaseProcessor(params),
    olsLeft(WINDOW_SIZE),
    olsRight(WINDOW_SIZE),
    windowFunction(WINDOW_SIZE, juce::dsp::WindowingFunction<float>::hann)
{
    paramMngr.registerParameterCallback("gain",
        [this] (float value, bool)
        {
        });
}

PhaseVocoderProcessor::~PhaseVocoderProcessor()
{
}

void PhaseVocoderProcessor::prepare(double, int maxBufferSize)
{
    olsLeft.prepare(static_cast<size_t>(maxBufferSize));
    olsRight.prepare(static_cast<size_t>(maxBufferSize));
}

void PhaseVocoderProcessor::process(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    const auto numSamples { static_cast<size_t>(buffer.getNumSamples()) };
    const auto numChannels { static_cast<size_t>(buffer.getNumChannels()) };
    std::array<float, WINDOW_SIZE> frameBuffer;

    olsLeft.writeNewBuffer(buffer.getReadPointer(0), numSamples);
    while (olsLeft.readReadyFrame(frameBuffer.data()))
    {
        windowFunction.multiplyWithWindowingTable(frameBuffer.data(), WINDOW_SIZE);
    }

    if (numChannels > 1)
    {
        olsRight.writeNewBuffer(buffer.getReadPointer(1), numSamples);
        while (olsRight.readReadyFrame(frameBuffer.data()))
        {
            windowFunction.multiplyWithWindowingTable(frameBuffer.data(), WINDOW_SIZE);
        }
    }

    buffer.clear();
}

juce::AudioProcessorEditor* PhaseVocoderProcessor::createEditor()
{
    return new PhaseVocoderEditor(*this);
}

CREATE_PLUGIN(PhaseVocoderProcessor)
