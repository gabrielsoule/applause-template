#include "ApplauseTemplatePlugin.h"

#include "ApplauseTemplateEditor.h"

ApplauseTemplatePlugin::ApplauseTemplatePlugin(const clap_plugin_descriptor_t* descriptor, const clap_host_t* host)
    : PluginBase(descriptor, host),
      params_(host),
      gui_ext_(host, [this]() { return std::make_unique<ApplauseTemplateEditor>(&params_); })
{
    // Let's add a MIDI input port
    note_ports_.addInput(applause::NotePortConfig::midi("MIDI In"));

    // Now we add an audio output port
    // It's also possible to use a QOL shortcut:
    // audio_ports_.addOutput(applause::AudioPortConfig::mainStereo("Main Out"));
    // But, in this example, we spell it all out in the interest of good pedagogy.
    audio_ports_.addOutput(applause::AudioPortConfig{
        .name = "Main Out",
        .channel_count = 2,
        .port_type = CLAP_PORT_STEREO,
        .flags = CLAP_AUDIO_PORT_IS_MAIN
    });

    params_.registerParam(applause::ParamConfig{
        .string_id = "test_param",
        .name = "Test Parameter",
        .short_name = "Test",
        .min_value = 0.0f,
        .max_value = 100.0f,
        .default_value = 50.0f,
        .is_stepped = false // if true, the parameter will be snapped to integer values -- good for enums/mode selection
    });


    // Configure state extension callbacks for parameter persistence
    state_.setSaveCallback([this](auto& ar)
    {
        return params_.saveToStream(ar);
    });

    state_.setLoadCallback([this](auto& ar)
    {
        return params_.loadFromStream(ar);
    });


    // Register extensions with the plugin. The extensions will bind themselves directly to CLAP's C ABI.
    registerExtension(note_ports_);
    registerExtension(audio_ports_);
    registerExtension(state_);
    registerExtension(params_);
    registerExtension(gui_ext_);
}

bool ApplauseTemplatePlugin::init() noexcept
{
    LOG_INFO("ApplauseExample::init()");
    return true;
}

void ApplauseTemplatePlugin::destroy() noexcept
{
    LOG_INFO("ApplauseExample::destroy()");
}

bool ApplauseTemplatePlugin::activate(double sampleRate, uint32_t minFrameCount, uint32_t maxFrameCount) noexcept
{
    LOG_INFO("ApplauseExample::activate() - sampleRate: {}", sampleRate);
    return true;
}

void ApplauseTemplatePlugin::deactivate() noexcept
{
    LOG_INFO("ApplauseExample::deactivate()");
}

clap_process_status ApplauseTemplatePlugin::process(const clap_process_t* process) noexcept
{
    // Let the parameter module process events.
    params_.processEvents(process->in_events, process->out_events);

    return CLAP_PROCESS_SLEEP;
}
