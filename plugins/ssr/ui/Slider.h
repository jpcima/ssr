#pragma once
#include "Cairo.hpp"
#include <string>
#include <functional>
class FontEngine;

class Slider : public CairoSubWidget {
public:
    Slider(SubWidget* const parent, FontEngine &fontEngine);
    Slider(TopLevelWidget* const parent, FontEngine &fontEngine);

    double value() const noexcept { return fValue; }
    void setValue(double value);

    bool valueNotified() const { return fValueNotify; }
    void setValueNotified(bool notified);

    void setValueBounds(double v1, double v2);
    void setNumSteps(unsigned numSteps);

    std::function<void(double)> ValueChangedCallback;
    std::function<std::string(double)> FormatCallback;

protected:
    bool onMouse(const MouseEvent &event) override;
    bool onMotion(const MotionEvent &event) override;
    bool onScroll(const ScrollEvent &event) override;
    void onCairoDisplay(const CairoGraphicsContext& context) override;

private:
    double clampToBounds(double value);

private:
    FontEngine &fFontEngine;
    double fValue = 0;
    double fValueBound1 = 0, fValueBound2 = 1;
    unsigned fNumSteps = 100;
    bool fValueNotify = true;
    bool fIsDragging = false;
};
