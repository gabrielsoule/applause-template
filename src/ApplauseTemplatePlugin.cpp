#include "ApplauseTemplatePlugin.h"

#include "ApplauseTemplateEditor.h"
#include <applause/util/Json.h>


// When your plugin is instantiated, you'll have to pass the host pointer to your extensions.
// Otherwise, they won't know how to communicate with the DAW!
// The GUI extension is a little special: it doesn't actually implement a GUI;
// rather, it's a C++ glue layer that facilitates communication between a
// abstract GUI editor and the host. Therefore, we pass in a factory lambda
// that the extension can use to create editor instances when the host asks.
ApplauseTemplatePlugin::ApplauseTemplatePlugin(const clap_plugin_descriptor_t* descriptor, const clap_host_t* host)
    : PluginBase(descriptor, host),
      gui_ext_( [this] { return std::make_unique<ApplauseTemplateEditor>(&params_); })
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

    // This is how you register parameters. Pretty simpler.
    params_.registerParam(applause::ParamConfig{
        .string_id = "test_param",
        .name = "Test Parameter",
        .short_name = "Test",
        .min_value = 0.0f,
        .max_value = 100.0f,
        .default_value = 50.0f,
        .is_stepped = false // if true, the parameter will be snapped to integer values -- good for enums/mode selection
    });


    // If you want to save/load your plugin's state, you'll need to furnish the
    // StateExtension with the appropriate callbacks.
    // ParamsExtension has some helper functions that are very useful here.
    // You can also save and load our own arbitrary data, if you want.
    state_.setSaveCallback([this](applause::json& j)
    {
        params_.saveToJson(j["parameters"]);
        return true;
    });

    state_.setLoadCallback([this](const applause::json& j)
    {
        if (j.contains("parameters")) {
            params_.loadFromJson(j["parameters"]);
        }
        return true;
    });


    // Register extensions with the plugin.
    // The extensions will bind themselves directly to CLAP's C ABI.
    // This is VERY IMPORTANT! If you don't tell Applause about your extensions,
    // they won't do anything!
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

bool ApplauseTemplatePlugin::activate(const applause::ProcessInfo& info) noexcept
{
    LOG_INFO("ApplauseExample::activate() - sampleRate: {}", info.sample_rate);
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
