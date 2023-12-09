// SPDX-License-Identifier: BSD-2-Clause

// This code is part of the sfizz library and is licensed under a BSD 2-clause
// license. You should have receive a LICENSE file along with the code.
// If not, contact the sfizz maintainers at https://github.com/sfztools/sfizz

#include "ResonantArray.h"
#include "ResonantString.h"
#include <algorithm>

namespace sfz {
namespace fx {

ResonantArrayScalar::ResonantArrayScalar()
{
}

ResonantArrayScalar::~ResonantArrayScalar()
{
}

void ResonantArrayScalar::init(float sampleRate, unsigned maxStrings)
{
    ResonantString* strings = new ResonantString[maxStrings];
    _strings.reset(strings);

    for (unsigned i = 0; i < maxStrings; ++i) {
        ResonantString& rs = strings[i];
        rs.init(sampleRate);
    }
}

void ResonantArrayScalar::setup(
        unsigned numStrings,
        const float pitches[], const float bandwidths[],
        const float feedbacks[], const float gains[])
{
    ResonantString* strings = _strings.get();

    _numStrings = numStrings;

    for (unsigned i = 0; i < numStrings; ++i) {
        ResonantString& rs = strings[i];
        rs.clear();
        rs.setResonanceFrequency(pitches[i], bandwidths[i]);
        rs.setResonanceFeedback(feedbacks[i]);
        rs.setGain(gains[i]);
    }
}

void ResonantArrayScalar::clear()
{
    ResonantString* strings = _strings.get();
    const unsigned numStrings = _numStrings;

    for (unsigned i = 0; i < numStrings; ++i) {
        ResonantString& rs = strings[i];
        rs.clear();
    }
}

void ResonantArrayScalar::process(const float *inPtr, float *outPtr, unsigned numFrames)
{
    ResonantString* strings = _strings.get();
    const unsigned numStrings = _numStrings;

    std::fill(outPtr, outPtr + numFrames, 0.0f);

    for (unsigned is = 0; is < numStrings; ++is) {
        ResonantString& rs = strings[is];
        for (unsigned i = 0; i < numFrames; ++i)
            outPtr[i] += rs.process(inPtr[i]);
    }
}

} // namespace sfz
} // namespace fx
