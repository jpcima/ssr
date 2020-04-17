#include "State.h"
#include "StringIO.h"
#include <algorithm>
#include <cmath>

enum { kStateVersion = 0 };

SsrState SsrState::createDefault()
{
    SsrState state;

    state.numStrings = 88;

    for (unsigned i = 0; i < kMaxStrings; ++i) {
        int midiNote = i + 24;
        state.pitches[i] = 440.0 * std::exp2((midiNote - 69) * (1.0 / 12.0));
    }

    // 1 Hz works decently as compromise of selectivity/speed
    std::fill(state.bandwidths, state.bandwidths + kMaxStrings, 1.0f);

    // string feedbacks, expressed as time constants
    const float releaseTime = 50e-3f;
    std::fill(state.feedbackTimes, state.feedbackTimes + kMaxStrings, releaseTime);

    // for damping of the high frequencies
    std::fill(state.gains, state.gains + kMaxStrings, 1e-3f);

    return state;
}

std::string SsrState::toStateString() const
{
    std::string str;
    StringWriter wr(str);

    wr.encodeU32(kStateVersion);
    wr.encodeU32(numStrings);

    for (uint32_t i = 0; i < numStrings; ++i)
        wr.encodeF32(pitches[i]);
    for (uint32_t i = 0; i < numStrings; ++i)
        wr.encodeF32(bandwidths[i]);
    for (uint32_t i = 0; i < numStrings; ++i)
        wr.encodeF32(feedbackTimes[i]);
    for (uint32_t i = 0; i < numStrings; ++i)
        wr.encodeF32(gains[i]);

    return str;
}

bool SsrState::fromStateString(SsrState &state, const char *str)
{
    StringReader rd(str);

    uint32_t stateVersion;
    if (!rd.decodeU32(stateVersion))
        return false;

    if (stateVersion != kStateVersion)
        return false;

    if (!rd.decodeU32(state.numStrings))
        return false;

    if (state.numStrings > kMaxStrings)
        return false;

    for (uint32_t i = 0; i < state.numStrings; ++i) {
        if (!rd.decodeF32(state.pitches[i]))
            return false;
    }
    for (uint32_t i = 0; i < state.numStrings; ++i) {
        if (!rd.decodeF32(state.bandwidths[i]))
            return false;
    }
    for (uint32_t i = 0; i < state.numStrings; ++i) {
        if (!rd.decodeF32(state.feedbackTimes[i]))
            return false;
    }
    for (uint32_t i = 0; i < state.numStrings; ++i) {
        if (!rd.decodeF32(state.gains[i]))
            return false;
    }

    return true;
}
