#include <algorithm>
#include "Window.hpp"
#include "Cairo.hpp"
#include "StringsEditor.h"

StringsEditor::StringsEditor(TopLevelWidget *group)
    : CairoSubWidget(group),
      fBarColor(0.95f, 0.45f, 0.0f)
{
    fStringValues.resize(88);
}

StringsEditor::StringsEditor(SubWidget *group)
    : CairoSubWidget(group),
      fBarColor(0.95f, 0.45f, 0.0f)
{
    fStringValues.resize(88);
}

void StringsEditor::setBarColor(Color color)
{
    if (fBarColor == color)
        return;

    fBarColor = color;
    repaint();
}

void StringsEditor::setNumStrings(unsigned numStrings)
{
    if (fStringValues.size() == numStrings)
        return;

    fStringValues.resize(numStrings);
    repaint();
}

float StringsEditor::stringValue(uint32_t stringNum) const
{
    const uint32_t numStrings = fStringValues.size();
    if (stringNum >= numStrings)
        return 0.0;

    return fStringValues[stringNum];
}

void StringsEditor::setStringValue(uint32_t stringNum, float value)
{
    const uint32_t numStrings = fStringValues.size();
    if (stringNum >= numStrings || fStringValues[stringNum] == value)
        return;

    fStringValues[stringNum] = value;

    if (OnStringValueChanged)
        OnStringValueChanged(stringNum, value);

    repaint();
}

void StringsEditor::setStringValueBounds(float min, float max)
{
    if (fStringValueMin == min && fStringValueMax == max)
        return;

    fStringValueMin = min;
    fStringValueMax = max;

    for (float &value : fStringValues)
        value = std::max(min, std::min(max, value));

    repaint();
}

void StringsEditor::onCairoDisplay(const CairoGraphicsContext& context)
{
    cairo_t* const cr = context.handle;
    cairo_save(cr);

    const int w = getWidth();
    const int h = getHeight();

    cairo_rectangle(cr, 0.0, 0.0, w, h);
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    cairo_fill(cr);

    const uint32_t numStrings = fStringValues.size();
    const double xsep = w / (1.0 + numStrings);

    cairo_set_line_width(cr, 2.0);

    const Color barColor = fBarColor;

    const float min = fStringValueMin;
    const float max = fStringValueMax;

    for (uint32_t i = 0; i < numStrings; ++i) {
        const double xstr = xsep * (1 + i);
        const float value = fStringValues[i];
        const float ratio = (value - min) / (max - min);

        cairo_new_path(cr);
        cairo_move_to(cr, xstr, 0.0);
        cairo_line_to(cr, xstr, h);
        cairo_set_source_rgb(cr, 0.25, 0.25, 0.25);
        cairo_stroke(cr);

        cairo_new_path(cr);
        cairo_move_to(cr, xstr, h * (1 - ratio));
        cairo_line_to(cr, xstr, h);
        cairo_set_source_rgb(cr, barColor.red, barColor.green, barColor.blue);
        cairo_stroke(cr);
    }

    cairo_restore(cr);
}

bool StringsEditor::onMouse(const MouseEvent &event)
{
    const int w = getWidth();
    const int h = getHeight();

    if (event.button == 1 && event.press) {
        double mx = event.pos.getX();
        double my = event.pos.getY();
        if (mx >= 0 && mx < w && my >= 0 && my < h) {
            fDragging = true;
            fLastStringMouseEdited = -1;
            editStringByMouse(event.pos);
        }
    }
    else if (event.button == 1 && !event.press && fDragging) {
        fDragging = false;
        return true;
    }

    return false;
}

bool StringsEditor::onMotion(const MotionEvent &event)
{
    if (fDragging) {
        editStringByMouse(event.pos);
        return true;
    }

    return false;
}

double StringsEditor::stringNumToXPos(double s) const
{
    const int w = getWidth();
    const uint32_t numStrings = fStringValues.size();
    const double xsep = w / (1.0 + numStrings);
    return xsep * (s + 1);
}

double StringsEditor::xPosToStringNum(double x) const
{
    const int w = getWidth();
    const uint32_t numStrings = fStringValues.size();
    const double xsep = w / (1.0 + numStrings);
    return x / xsep - 1;
}

void StringsEditor::editStringByMouse(Point<double> pos)
{
    int32_t stringNum = std::lrint(xPosToStringNum(pos.getX()));
    const uint32_t numStrings = fStringValues.size();

    stringNum = std::min(numStrings, (uint32_t)std::max(0, stringNum));

    const float min = fStringValueMin;
    const float max = fStringValueMax;

    const float ratio = std::max(0.0f, std::min(1.0f, 1 - (float)pos.getY() / getHeight()));
    const float value = min + ratio * (max - min);

    const int32_t prevStringNum = fLastStringMouseEdited;
    fLastStringMouseEdited = stringNum;

    if (prevStringNum != -1 && prevStringNum != stringNum) {
        // interpolate intermediate strings
        const float prevValue = fStringValues[prevStringNum];

        const int32_t stringStep = (prevStringNum < stringNum) ? +1 : -1;
        const int32_t stringDistance = std::abs(stringNum - prevStringNum);

        for (int32_t i = 1; i < stringDistance; ++i) {
            const float ratio = i * (1.0 / stringDistance);
            setStringValue(
                prevStringNum + i * stringStep,
                ratio * prevValue + (1 - ratio) * value);
        }
    }

    setStringValue(stringNum, value);
}
