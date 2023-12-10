/*
 * ssr audio effect based on DISTRHO Plugin Framework (DPF)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (C) 2020 Jean Pierre Cimalando <jpcima@protonmail.com>
 */

#ifndef DISTRHO_PLUGIN_INFO_H
#define DISTRHO_PLUGIN_INFO_H

#define DISTRHO_PLUGIN_BRAND "JPC"
#define DISTRHO_PLUGIN_NAME "ssr"
#define DISTRHO_PLUGIN_URI "http://jpcima.sdf1.org/plugins/ssr"

#define DISTRHO_PLUGIN_HAS_UI 1
#define DISTRHO_UI_USE_CAIRO 1
#define DISTRHO_UI_USE_NANOVG 0

#define DISTRHO_PLUGIN_IS_RT_SAFE 1
#define DISTRHO_PLUGIN_NUM_INPUTS 2
#define DISTRHO_PLUGIN_NUM_OUTPUTS 2
#define DISTRHO_PLUGIN_WANT_TIMEPOS 0
#define DISTRHO_PLUGIN_WANT_PROGRAMS 0
#define DISTRHO_PLUGIN_WANT_MIDI_INPUT 0
#define DISTRHO_PLUGIN_WANT_MIDI_OUTPUT 0

#define DISTRHO_PLUGIN_WANT_STATE 1
#define DISTRHO_PLUGIN_WANT_FULL_STATE 1

#define DISTRHO_PLUGIN_LV2_CATEGORY "lv2:SimulatorPlugin"

#endif  // DISTRHO_PLUGIN_INFO_H
