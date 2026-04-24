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
    olaLeft(WINDOW_SIZE),
    olaRight(WINDOW_SIZE),
    phaseLeft(WINDOW_SIZE),
    phaseRight(WINDOW_SIZE),
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

    olaLeft.prepare(static_cast<size_t>(maxBufferSize));
    olaRight.prepare(static_cast<size_t>(maxBufferSize));
}

void PhaseVocoderProcessor::process(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    const auto numSamples { static_cast<size_t>(buffer.getNumSamples()) };
    const auto numChannels { static_cast<size_t>(buffer.getNumChannels()) };
    std::array<float, WINDOW_SIZE> analysisFrameBuffer;
    std::array<float, WINDOW_SIZE> synthesisFrameBuffer;


    olsLeft.writeNewBuffer(buffer.getReadPointer(0), numSamples);
    while (olsLeft.readReadyFrame(analysisFrameBuffer.data()))
    {
        windowFunction.multiplyWithWindowingTable(analysisFrameBuffer.data(), WINDOW_SIZE);
        phaseLeft.processFrame(synthesisFrameBuffer.data(), analysisFrameBuffer.data());
        windowFunction.multiplyWithWindowingTable(synthesisFrameBuffer.data(), WINDOW_SIZE);
        olaLeft.writeNewFrame(synthesisFrameBuffer.data());
    }

    const auto readSamplesLeft { olaLeft.readyReadBuffer(buffer.getWritePointer(0), numSamples) };
    if ((readSamplesLeft > 0) && (readSamplesLeft < numSamples))
    {
        const auto offsetSamples { numSamples - readSamplesLeft };
        std::memmove(buffer.getWritePointer(0) + offsetSamples, buffer.getReadPointer(0), offsetSamples * sizeof(float));
        std::memset(buffer.getWritePointer(0), 0, offsetSamples * sizeof(float));
    }


    if (numChannels > 1)
    {
        olsRight.writeNewBuffer(buffer.getReadPointer(1), numSamples);
        while (olsRight.readReadyFrame(analysisFrameBuffer.data()))
        {
            windowFunction.multiplyWithWindowingTable(analysisFrameBuffer.data(), WINDOW_SIZE);
            phaseRight.processFrame(synthesisFrameBuffer.data(), analysisFrameBuffer.data());
            windowFunction.multiplyWithWindowingTable(synthesisFrameBuffer.data(), WINDOW_SIZE);
            olaRight.writeNewFrame(synthesisFrameBuffer.data());
        }

        const auto readSamplesRight { olaRight.readyReadBuffer(buffer.getWritePointer(1), numSamples) };
        if ((readSamplesRight > 0) && (readSamplesRight < numSamples))
        {
            const auto offsetSamples { numSamples - readSamplesRight };
            std::memmove(buffer.getWritePointer(1) + offsetSamples, buffer.getReadPointer(1), offsetSamples * sizeof(float));
            std::memset(buffer.getWritePointer(1), 0, offsetSamples * sizeof(float));
        }
    }
}

juce::AudioProcessorEditor* PhaseVocoderProcessor::createEditor()
{
    return new PhaseVocoderEditor(*this);
}

CREATE_PLUGIN(PhaseVocoderProcessor)
