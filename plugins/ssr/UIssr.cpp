#include "UIssr.h"
#include "State.h"
#include "Definitions.h"
#include "ui/StringsEditor.h"
#include "ui/Slider.h"
#include "ui/FontEngine.h"
#include "Window.hpp"
#include <cstring>

static constexpr bool kGainsEditLogarithmic = true;

// -----------------------------------------------------------------------

static constexpr uint8_t fontRegular[] = {
    #include "fonts/liberation/LiberationSans-Regular.ttf.h"
};

// -----------------------------------------------------------------------
// Init / Deinit

UIssr::UIssr()
    : UI(600, 400),
      fState(new SsrState(SsrState::createDefault()))
{
    FontEngine *fe = new FontEngine;
    fFontEngine.reset(fe);
    fe->addFont("regular", fontRegular, sizeof(fontRegular));

    fGainEdit = new StringsEditor(this);
    fGainEdit->setAbsolutePos(20, 40);
    fGainEdit->setSize(560, 120);
    fGainEdit->setBarColor(Color(0.95f, 0.45f, 0.0f));
    if (kGainsEditLogarithmic)
        fGainEdit->setStringValueBounds(kGainMinDb, kGainMaxDb);
    else
        fGainEdit->setStringValueBounds(gainLog2Lin(kGainMinDb), gainLog2Lin(kGainMaxDb));
    fGainEdit->OnStringValueChanged = [this](uint32_t stringNum, float value) {
        if (kGainsEditLogarithmic)
            fState->gains[stringNum] = gainLog2Lin(value);
        else
            fState->gains[stringNum] = value;
        fMustResendState = true;
    };

    fTimeEdit = new StringsEditor(this);
    fTimeEdit->setAbsolutePos(20, 200);
    fTimeEdit->setSize(560, 120);
    fTimeEdit->setBarColor(Color(0.0f, 0.95f, 0.45f));
    fTimeEdit->setStringValueBounds(kFeedbackTimeMin, kFeedbackTimeMax);
    fTimeEdit->OnStringValueChanged = [this](uint32_t stringNum, float value) {
        fState->feedbackTimes[stringNum] = value;
        fMustResendState = true;
    };

    fDepthSlider = new Slider(this, *fe);
    fDepthSlider->setAbsolutePos(20, 360);
    fDepthSlider->setSize(560, 20);
    fDepthSlider->ValueChangedCallback = [this](float value) {
        setParameterValue(Pluginssr::pidSustainAmount, value);
    };

    updateUiState();
}

UIssr::~UIssr()
{
}

// -----------------------------------------------------------------------
// DSP/Plugin callbacks

/**
  A parameter has changed on the plugin side.
  This is called by the host to inform the UI about parameter changes.
*/
void UIssr::parameterChanged(uint32_t index, float value)
{
    DISTRHO_SAFE_ASSERT_RETURN(index < Pluginssr::paramCount, );

    switch (index) {
    case Pluginssr::pidSustainAmount:
        fDepthSlider->setValue(value);
        break;
    }

    (void)value;
}

/**
  Optional callback to inform the UI about a sample rate change on the plugin side.
*/
void UIssr::sampleRateChanged(double newSampleRate)
{
    (void)newSampleRate;
}

void UIssr::stateChanged(const char *key, const char *value)
{
    if (!std::strcmp(key, "strs")) {
        SsrState state;
        if (!SsrState::fromStateString(state, value))
            return;

        *fState = state;
        updateUiState();
    }
}

// -----------------------------------------------------------------------
// Optional UI callbacks

/**
  Idle callback.
  This function is called at regular intervals.
*/
void UIssr::uiIdle()
{
    if (fMustResendState) {
        setState("strs", fState->toStateString().c_str());
        fMustResendState = false;
    }
}

/**
  Window reshape function, called when the parent window is resized.
*/
void UIssr::uiReshape(uint width, uint height)
{
    (void)width;
    (void)height;
}

// -----------------------------------------------------------------------
// Widget callbacks


/**
  A function called to draw the view contents.
*/
void UIssr::onCairoDisplay(const CairoGraphicsContext& context)
{
    cairo_t* const cr = context.handle;

    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_paint(cr);

    FontEngine &fe = *fFontEngine;

    Font fontLabel;
    fontLabel.color = ColorRGBA8{0xff, 0xff, 0xff, 0xff};
    fontLabel.name = "regular";
    fontLabel.size = 20.0;

    fe.drawInBox(
        cr, "Gain", fontLabel,
        Rect(fGainEdit->getAbsoluteX(), fGainEdit->getAbsoluteY() - 1.5 * fontLabel.size, 0, 0),
        kAlignTopLeft|kAlignInside);
    fe.drawInBox(
        cr, "Feedback", fontLabel,
        Rect(fTimeEdit->getAbsoluteX(), fTimeEdit->getAbsoluteY() - 1.5 * fontLabel.size, 0, 0),
        kAlignTopLeft|kAlignInside);
    fe.drawInBox(
        cr, "Depth", fontLabel,
        Rect(fDepthSlider->getAbsoluteX(), fDepthSlider->getAbsoluteY() - 1.5 * fontLabel.size, 0, 0),
        kAlignTopLeft|kAlignInside);
}


// -----------------------------------------------------------------------

void UIssr::updateUiState()
{
    const SsrState &state = *fState;
    const uint32_t numStrings = state.numStrings;

    fGainEdit->setNumStrings(numStrings);
    fTimeEdit->setNumStrings(numStrings);

    for (uint32_t i = 0; i < numStrings; ++i) {
        if (kGainsEditLogarithmic)
            fGainEdit->setStringValue(i, gainLin2Log(state.gains[i]));
        else
            fGainEdit->setStringValue(i, state.gains[i]);
        fTimeEdit->setStringValue(i, state.feedbackTimes[i]);
    }
}

// -----------------------------------------------------------------------

UI *DISTRHO::createUI()
{
    return new UIssr();
}
