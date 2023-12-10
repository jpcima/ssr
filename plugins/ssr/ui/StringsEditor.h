#pragma once
#include "Cairo.hpp"
#include "Color.hpp"
#include <functional>
#include <vector>

class StringsEditor : public CairoSubWidget {
public:
    explicit StringsEditor(TopLevelWidget *group);
    explicit StringsEditor(SubWidget *group);

    Color barColor() const { return fBarColor; }
    void setBarColor(Color color);

    uint32_t numStrings() const { return fStringValues.size(); }
    void setNumStrings(uint32_t numStrings);

    float stringValue(uint32_t stringNum) const;
    void setStringValue(uint32_t stringNum, float value);

    void setStringValueBounds(float min, float max);

    std::function<void(uint32_t, float)> OnStringValueChanged;

protected:
    void onCairoDisplay(const CairoGraphicsContext& context) override;
    bool onMouse(const MouseEvent &event) override;
    bool onMotion(const MotionEvent &event) override;

private:
    double stringNumToXPos(double s) const;
    double xPosToStringNum(double x) const;

    void editStringByMouse(Point<double> pos);

private:
    std::vector<float> fStringValues;
    float fStringValueMin = 0.0;
    float fStringValueMax = 1.0;
    bool fDragging = false;
    int32_t fLastStringMouseEdited = -1;
    Color fBarColor;
};
