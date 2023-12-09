// SPDX-License-Identifier: BSD-2-Clause

// This code is part of the sfizz library and is licensed under a BSD 2-clause
// license. You should have receive a LICENSE file along with the code.
// If not, contact the sfizz maintainers at https://github.com/sfztools/sfizz

#pragma once

namespace sfz {
namespace config {
    constexpr float defaultSampleRate { 48000 };
    constexpr int defaultSamplesPerBlock { 1024 };
} // namespace config

// Enable or disable SIMD accelerators by default
namespace SIMDConfig {
    constexpr unsigned int defaultAlignment { 16 };
}
}
