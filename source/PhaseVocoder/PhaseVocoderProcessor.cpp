#include "PhaseVocoderProcessor.h"
#include "PhaseVocoderEditor.h"

static const std::vector<mrta::ParameterInfo> params
{
    mrta::ParameterInfo("ratio", "Ratio", "X", 1.f, 0.5f, 2.f, 0.001f, 1.f)
};

PhaseVocoderProcessor::PhaseVocoderProcessor() :
    mrta::BaseProcessor(params),
    olsLeft(WINDOW_SIZE),
    olsRight(WINDOW_SIZE),
    olaLeft(WINDOW_SIZE),
    olaRight(WINDOW_SIZE),
    phaseLeft(WINDOW_SIZE),
    phaseRight(WINDOW_SIZE),
    windowFunction(WINDOW_SIZE, juce::dsp::WindowingFunction<float>::hann)
{
    registerParameterCallback("ratio",
        [this] (float value, bool)
        {
            const auto auxRatio = value;
            analysisHopSize = static_cast<size_t>(std::floor(BASE_HOP_SIZE / auxRatio));
            ratio = static_cast<double>(synthesisHopSize) / static_cast<double>(analysisHopSize);

            olsLeft.setHopSizeSamples(analysisHopSize);
            olsRight.setHopSizeSamples(analysisHopSize);
            phaseLeft.setAnalysisHopSize(analysisHopSize);
            phaseRight.setAnalysisHopSize(analysisHopSize);
        });
}

PhaseVocoderProcessor::~PhaseVocoderProcessor()
{
}

void PhaseVocoderProcessor::prepare(double, int maxBufferSize)
{
    olsLeft.prepare();
    olsRight.prepare();

    olaLeft.prepare();
    olaRight.prepare();

    phaseLeft.reset();
    phaseRight.reset();

    resamplerLeft.reset();
    resamplerRight.reset();

    olsLeft.setHopSizeSamples(analysisHopSize);
    olsRight.setHopSizeSamples(analysisHopSize);

    olaLeft.setHopSizeSamples(synthesisHopSize);
    olaRight.setHopSizeSamples(synthesisHopSize);

    phaseLeft.setAnalysisHopSize(analysisHopSize);
    phaseLeft.setSynthesisHopSize(synthesisHopSize);
    phaseRight.setAnalysisHopSize(analysisHopSize);
    phaseRight.setSynthesisHopSize(synthesisHopSize);

    resampleFracAcc = 0.0;

    resamplerBuffer.resize(2 * maxBufferSize);
    std::fill(resamplerBuffer.begin(), resamplerBuffer.end(), 0.f);
}

void PhaseVocoderProcessor::process(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    const auto numSamples { static_cast<size_t>(buffer.getNumSamples()) };
    const auto numChannels { static_cast<size_t>(buffer.getNumChannels()) };
    std::array<float, WINDOW_SIZE> analysisFrameBuffer;
    std::array<float, WINDOW_SIZE> synthesisFrameBuffer;

    auto resampleLengthFrac { numSamples * ratio };
    auto resampleLength { static_cast<size_t>(std::floor(resampleLengthFrac)) };
    resampleFracAcc += resampleLengthFrac - resampleLength;
    if (resampleFracAcc >= 1.0)
    {
        ++resampleLength;
        --resampleFracAcc;
    }

    const auto resamplerRatio { static_cast<double>(resampleLength) / static_cast<double>(numSamples) };

    olsLeft.writeNewBuffer(buffer.getReadPointer(0), numSamples);
    while (olsLeft.readReadyFrame(analysisFrameBuffer.data()))
    {
        windowFunction.multiplyWithWindowingTable(analysisFrameBuffer.data(), WINDOW_SIZE);
        phaseLeft.processFrame(synthesisFrameBuffer.data(), analysisFrameBuffer.data());
        windowFunction.multiplyWithWindowingTable(synthesisFrameBuffer.data(), WINDOW_SIZE);
        olaLeft.writeNewFrame(synthesisFrameBuffer.data());
    }

    olaLeft.readyReadBuffer(resamplerBuffer.data(), resampleLength);
    resamplerLeft.process(resamplerRatio, resamplerBuffer.data(), buffer.getWritePointer(0),
        static_cast<int>(numSamples), static_cast<int>(resampleLength), 1);

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

        olaRight.readyReadBuffer(resamplerBuffer.data(), resampleLength);
        resamplerRight.process(resamplerRatio, resamplerBuffer.data(), buffer.getWritePointer(1),
            static_cast<int>(numSamples), static_cast<int>(resampleLength), 1);
    }
}

juce::AudioProcessorEditor* PhaseVocoderProcessor::createEditor()
{
    return new PhaseVocoderEditor(*this);
}

CREATE_PLUGIN(PhaseVocoderProcessor)
