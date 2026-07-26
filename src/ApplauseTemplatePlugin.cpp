#include "ApplauseTemplatePlugin.h"

#include "ApplauseTemplateEditor.h"
#include <applause/util/Json.h>

#include <cmath>
#include <numbers>

namespace
{
    // One full cycle of a sine wave spans 2*pi radians.
    constexpr double kTwoPi = 2.0 * std::numbers::pi;

    // How loud our test tone is, where 1.0 is full scale. A full-scale sine is painfully loud,
    // so we back off considerably. Real plugins would expose this as a gain parameter.
    constexpr float kAmplitude = 0.2f;
}


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
    // This one sets the pitch of the test tone we generate down in process().
    params_.registerParam(applause::ParamConfig{
        .string_id = "frequency",
        .name = "Frequency",
        .short_name = "Freq",
        .unit = "Hz",
        .min_value = 20.0f,
        .max_value = 2000.0f,
        .default_value = 440.0f,
        .is_stepped = false, // if true, the parameter will be snapped to integer values -- good for enums/mode selection
        // Scaling only affects how the plain value maps onto the normalized 0-1 range that hosts and
        // knobs work in; process() always sees plain Hz. Pitch is perceived logarithmically, so a
        // frequency scaling gives the knob an even musical feel instead of cramming every useful
        // pitch into the first sliver of its travel.
        .scaling = applause::ValueScaling::frequency(20.0f, 2000.0f)
    });

    // Look up the audio-thread handle for the parameter we just registered, once, and cache it.
    frequency_param_ = &params_.getHandle("frequency");


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

    // Hang onto the sample rate; our oscillator needs it. This is also the place to size any
    // buffers you need, since the host tells you the largest block it will ever ask you to render.
    sample_rate_ = info.sample_rate;
    phase_ = 0.0;
    return true;
}

void ApplauseTemplatePlugin::deactivate() noexcept
{
    LOG_INFO("ApplauseExample::deactivate()");
}

void ApplauseTemplatePlugin::reset() noexcept
{
    phase_ = 0.0;
}

applause::ProcessStatus ApplauseTemplatePlugin::process(applause::ProcessContext& context) noexcept
{
    // Let the parameter module process events. Since the parameter system is an optional module,
    // the plugin has to wire it into the processing loop, since Applause doesn't hook up any modules
    // by default.
    params_.processEvents(context.inputEvents(), context.outputEvents());

    // Ask the context for a writable view of our output buffer. The template arguments mean
    // "32-bit float samples, at most 2 channels"; the view itself knows how many channels the host
    // actually handed us, which may be fewer. An empty view means the host gave us nothing usable.
    // Since we requested a stereo setup in our audio ports extension, we are expecting a stereo output.
    auto output = context.output<float, 2>();
    if (output.numChannels() == 0) return applause::ProcessStatus::Continue;

    const double frequency = frequency_param_->getValue();
    const double phase_increment = kTwoPi * frequency / sample_rate_;

    for (std::size_t frame = 0; frame < output.numFrames(); ++frame)
    {
        const float sample = kAmplitude * static_cast<float>(std::sin(phase_));
        for (std::size_t channel = 0; channel < output.numChannels(); ++channel)
            output.store(channel, frame, sample);

        phase_ += phase_increment;
        if (phase_ >= kTwoPi) phase_ -= kTwoPi;
    }

    // In this simple plugin, we always return Continue; more complex plugins may wish to
    // return other status codes depending on their state.. See ProcessStatus for more info.
    return applause::ProcessStatus::Continue;
}
