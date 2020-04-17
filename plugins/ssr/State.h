#pragma once
#include <string>
#include <cstdint>

enum { kMaxStrings = 104 };

struct SsrState {
    uint32_t numStrings;
    float pitches[kMaxStrings];
    float bandwidths[kMaxStrings];
    float feedbackTimes[kMaxStrings];
    float gains[kMaxStrings];

    static SsrState createDefault();

    std::string toStateString() const;
    static bool fromStateString(SsrState &state, const char *string);
};
