#pragma once

#include "ui/ApplauseEditor.h"
#include "ui/components/GenericParameterUI.h"
#include "ui/components/ParamKnob.h"
#include "extensions/ParamsExtension.h"
#include <memory>
#include <functional>

class ApplauseTemplateEditor : public applause::ApplauseEditor
{
public:
    explicit ApplauseTemplateEditor(applause::ParamsExtension* params);
    ~ApplauseTemplateEditor() override = default;
    
    void draw(visage::Canvas& canvas) override;
    void resized() override;
    
private:
    std::unique_ptr<applause::ParamKnob> test_param_knob_;
    applause::ParamsExtension* params_;
};
