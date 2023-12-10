#include "TextLabel.h"
#include "Window.hpp"
#include "Cairo.hpp"

TextLabel::TextLabel(TopLevelWidget *group, FontEngine &fontEngine)
    : CairoSubWidget(group),
      fFontEngine(fontEngine)
{
}

TextLabel::TextLabel(SubWidget *group, FontEngine &fontEngine)
    : CairoSubWidget(group),
      fFontEngine(fontEngine)
{
}

void TextLabel::setFont(const Font &font)
{
    if (fFont == font)
        return;

    fFont = font;
    repaint();
}

void TextLabel::setText(std::string text)
{
    if (fText == text)
        return;

    fText = std::move(text);
    repaint();
}

void TextLabel::setAlignment(int align)
{
    if (fAlign == align)
        return;

    fAlign = align;
    repaint();
}

void TextLabel::onCairoDisplay(const CairoGraphicsContext& context)
{
    cairo_t* const cr = context.handle;
    FontEngine &fe = fFontEngine;

    Rect box{0, 0, (int)getWidth(), (int)getHeight()};
    fe.drawInBox(cr, fText.c_str(), fFont, box, fAlign);
}
