#pragma once
#include "DistrhoUI.hpp"
#include "Pluginssr.h"
#include <vector>
#include <memory>
class StringsEditor;
class Slider;
class SsrState;
class FontEngine;

class UIssr : public UI {
public:
    UIssr();
    ~UIssr();

protected:
    void parameterChanged(uint32_t, float value) override;
    void sampleRateChanged(double newSampleRate) override;
    void stateChanged(const char *key, const char *value) override;

    void uiIdle() override;
    void uiReshape(uint width, uint height) override;

    void onCairoDisplay(const CairoGraphicsContext& context) override;

private:
    void updateUiState();

private:
    std::unique_ptr<FontEngine> fFontEngine;

    ScopedPointer<StringsEditor> fGainEdit;
    ScopedPointer<StringsEditor> fTimeEdit;
    ScopedPointer<Slider> fDepthSlider;

    std::unique_ptr<SsrState> fState;
    bool fMustResendState = false;

private:
    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UIssr)
};
