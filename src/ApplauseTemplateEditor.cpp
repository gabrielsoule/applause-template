#include "ApplauseTemplateEditor.h"
#include "util/DebugHelpers.h"
#include <visage_graphics/canvas.h>
#include "embedded/fonts.h"

using namespace visage::dimension;

ApplauseExampleEditor::ApplauseExampleEditor(applause::ParamsExtension* params)
    : Editor(params), params_(params)
{
    if (params_)
    {
        test_param_knob_ = std::make_unique<applause::ParamKnob>(params_->getInfo("test_param"));
        addChild(test_param_knob_.get());
    }
}

void ApplauseExampleEditor::draw(visage::Canvas& canvas)
{
    // Black background
    canvas.setColor(0xFF000000);
    canvas.fill(0, 0, width(), height());

    const visage::Font font(20, visage::fonts::DroidSansMono_ttf);
    canvas.setColor(0xFFFFFFFF);
    canvas.text("Hello, CLAP!", font, visage::Font::kCenter,
                10, 10, 200, 70);
}

void ApplauseExampleEditor::resized()
{
    test_param_knob_->setBounds(440, 80, 50, 50 + 20);
}
