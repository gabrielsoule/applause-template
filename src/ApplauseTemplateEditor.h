#pragma once
#include <applause/ui/ApplauseEditor.h>
#include <applause/ui/components/ParamKnob.h>

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
