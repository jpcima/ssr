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

    void onDisplay() override;

    bool onKeyboard(const KeyboardEvent &ev) override;
    bool onSpecial(const SpecialEvent &ev) override;
    bool onMouse(const MouseEvent &ev) override;
    bool onMotion(const MotionEvent &ev) override;
    bool onScroll(const ScrollEvent &ev) override;

private:
    void updateUiState();

private:
    template <class W, class... A>
    W *makeSubwidget(A &&... args)
    {
        W *w = new W(std::forward<A>(args)...);
        fSubWidgets.push_back(std::unique_ptr<Widget>(w));
        return w;
    }

    std::vector<std::unique_ptr<Widget>> fSubWidgets;
    std::unique_ptr<FontEngine> fFontEngine;

    StringsEditor *fGainEdit = nullptr;
    StringsEditor *fTimeEdit = nullptr;
    Slider *fDepthSlider = nullptr;

    std::unique_ptr<SsrState> fState;
    bool fMustResendState = false;

private:
    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UIssr)
};
