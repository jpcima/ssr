#pragma once
#include "Cairo.hpp"
#include "ui/FontEngine.h"

class TextLabel : public CairoSubWidget {
public:
    TextLabel(TopLevelWidget *group, FontEngine &fontEngine);
    TextLabel(SubWidget *group, FontEngine &fontEngine);

    const Font &font() const { return fFont; };
    void setFont(const Font &font);

    const std::string &text() const { return fText; }
    void setText(std::string text);

    int alignment() const { return fAlign; }
    void setAlignment(int align);

protected:
    void onCairoDisplay(const CairoGraphicsContext& context) override;

private:
    FontEngine &fFontEngine;
    Font fFont;
    std::string fText;
    int fAlign = kAlignLeft|kAlignTop|kAlignInside;
};
