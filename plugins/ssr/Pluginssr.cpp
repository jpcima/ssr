#include "Pluginssr.h"
#include "State.h"
#include "ResonantArray.h"
#include "ResonantArraySSE.h"
#include "ResonantArrayAVX.h"
#include "Config.h"
#include "cpuid/cpuinfo.hpp"
#include "blink/DenormalDisabler.h"
#include <algorithm>
#include <cstring>
#include <cmath>

Pluginssr::Pluginssr()
    : Plugin(paramCount, 0, stateCount),
      fState(new SsrState(SsrState::createDefault())),
      fFeedbacks(new float[kMaxStrings]),
      fTemp1(new float[sfz::config::defaultSamplesPerBlock]),
      fTemp2(new float[sfz::config::defaultSamplesPerBlock])
{
    sfz::fx::ResonantArray *array = nullptr;
#if SFIZZ_CPU_FAMILY_X86_64 || SFIZZ_CPU_FAMILY_I386
    cpuid::cpuinfo cpuInfo;
    if (cpuInfo.has_avx())
        array = new sfz::fx::ResonantArrayAVX;
    else if (cpuInfo.has_sse())
        array = new sfz::fx::ResonantArraySSE;
#endif
    if (!array)
        array = new sfz::fx::ResonantArrayScalar;
    fStringsArray.reset(array);

    /// init parameters
    for (uint32_t i = 0; i < paramCount; ++i) {
        Parameter p;
        initParameter(i, p);
        fParams[i] = p.ranges.def;
    }

    ///
    sampleRateChanged(getSampleRate());
}

// -----------------------------------------------------------------------
// Init

void Pluginssr::initParameter(uint32_t index, Parameter &parameter)
{
    DISTRHO_SAFE_ASSERT_RETURN(index < paramCount, );

    switch (index) {
    case pidSustainAmount:
        parameter.symbol = "depth";
        parameter.name = "Depth";
        parameter.ranges = ParameterRanges(1.0, 0.0, 1.0);
        parameter.hints = kParameterIsAutomatable|kParameterIsLogarithmic;
        break;
    }
}

// -----------------------------------------------------------------------
// Internal data

/**
  Optional callback to inform the plugin about a sample rate change.
*/
void Pluginssr::sampleRateChanged(double newSampleRate)
{
    fSampleRate = newSampleRate;
    fStringsArray->init(newSampleRate, kMaxStrings);

    fStringsNeedSetup.store(1);
}

/**
  Get the current value of a parameter.
*/
float Pluginssr::getParameterValue(uint32_t index) const
{
    return fParams[index];
}

/**
  Change a parameter value.
*/
void Pluginssr::setParameterValue(uint32_t index, float value)
{
    DISTRHO_SAFE_ASSERT_RETURN(index < paramCount, );

    fParams[index] = value;

    switch (index) {
    }
}

void Pluginssr::initState(uint32_t index, String &stateKey, String &defaultStateValue)
{
    DISTRHO_SAFE_ASSERT_RETURN(index < stateCount, );

    switch (index) {
    case 0:
        stateKey = "strs";
        defaultStateValue = SsrState::createDefault().toStateString().c_str();
        break;
    }
}

void Pluginssr::setState(const char *key, const char *value)
{
    if (!std::strcmp(key, "strs")) {
        SsrState state;
        if (SsrState::fromStateString(state, value)) {
            *fState = state;
            fStringsNeedSetup.store(1);
        }
    }
}

String Pluginssr::getState(const char *key) const
{
    String value;

    if (!std::strcmp(key, "strs"))
        value = fState->toStateString().c_str();

    return value;
}

// -----------------------------------------------------------------------
// Process

void Pluginssr::activate()
{
    // plugin is activated
    fStringsArray->clear();
}


void Pluginssr::run(const float **inputs, float **outputs, uint32_t totalFrames)
{
    WebCore::DenormalDisabler disableDenormals;

    constexpr uint32_t maxFrames = sfz::config::defaultSamplesPerBlock;

    for (uint32_t frameIndex = 0; frameIndex < totalFrames;) {
        uint32_t currentFrames = std::min(totalFrames - frameIndex, maxFrames);

        const float *currentInputs[DISTRHO_PLUGIN_NUM_INPUTS];
        float *currentOutputs[DISTRHO_PLUGIN_NUM_OUTPUTS];

        for (uint32_t i = 0; i < DISTRHO_PLUGIN_NUM_INPUTS; ++i)
            currentInputs[i] = inputs[i] + frameIndex;

        for (uint32_t i = 0; i < DISTRHO_PLUGIN_NUM_OUTPUTS; ++i)
            currentOutputs[i] = outputs[i] + frameIndex;

        runWithinBufferLimits(currentInputs, currentOutputs, currentFrames);
        frameIndex += currentFrames;
    }
}

void Pluginssr::runWithinBufferLimits(const float **inputs, float **outputs, uint32_t numFrames)
{
    if (fStringsNeedSetup.exchange(0))
        updateStringsSetup();

    const float *inputL = inputs[0];
    const float *inputR = inputs[1];

    // mix down the stereo signal to create the resonator excitation source
    float *resInput = fTemp1.get();
    for (uint32_t i = 0; i < numFrames; ++i)
        resInput[i] = (inputL[i] + inputR[i]) * (float)M_SQRT1_2;

    // generate the strings summed into a common buffer
    float *resOutput = fTemp2.get();
    sfz::fx::ResonantArray &array = *fStringsArray;
    array.process(resInput, resOutput, numFrames);

    // mix the resonator into the output
    float *outputL = outputs[0];
    float *outputR = outputs[1];
    const float sustainAmount = fParams[pidSustainAmount];
    for (uint32_t i = 0; i < numFrames; ++i) {
        outputL[i] = inputL[i] + sustainAmount * resOutput[i];
        outputR[i] = inputR[i] + sustainAmount * resOutput[i];
    }
}

void Pluginssr::updateStringsSetup()
{
    const double sampleRate = getSampleRate();

    ///
    const float *feedbackTimes = fState->feedbackTimes;
    float *feedbacks = fFeedbacks.get();
    for (unsigned i = 0; i < kMaxStrings; ++i)
        feedbacks[i] = std::exp(-6.91 / (feedbackTimes[i] * sampleRate));

    ///
    const SsrState &state = *fState;
    fStringsArray->setup(state.numStrings, state.pitches, state.bandwidths, feedbacks, state.gains);
}

// -----------------------------------------------------------------------

Plugin *DISTRHO::createPlugin()
{
    return new Pluginssr();
}
