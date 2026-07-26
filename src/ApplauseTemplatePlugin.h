#pragma once
#include <applause/core/PluginBase.h>
#include <applause/extensions/AudioPortsExtension.h>
#include <applause/extensions/GUIExtension.h>
#include <applause/extensions/NotePortsExtension.h>
#include <applause/extensions/ParamsExtension.h>
#include <applause/extensions/StateExtension.h>


class ApplauseTemplatePlugin : public applause::PluginBase
{
public:
    ApplauseTemplatePlugin(const clap_plugin_descriptor_t* descriptor, const clap_host_t* host);
    ~ApplauseTemplatePlugin() override = default;

    // Called when an instance of your plugin is instantiated by the host
    bool init() noexcept override;

    // Called when the host deletes your plugin instance
    void destroy() noexcept override;

    // Called when a sample rate and buffer size is established; similar to JUCE's prepareToPlay
    // Can be called many times over a plugin instance's lifetime
    bool activate(const applause::ProcessInfo& info) noexcept override;

    // Called when the plugin is deactivated and no longer expected to process audio, but not deleted
    void deactivate() noexcept override;

    // Called when the host wants the plugin to drop any state left over from previous audio,
    // e.g. after a transport jump. Clear out filter memory, oscillator phase, envelopes, etc.
    void reset() noexcept override;

    // The main audio processing function. This is the heart of your plugin.
    // Here, you'll perform DSP, respond to events, and output audio samples and/or MIDI notes.
    // Everything you need for the block lives on the ProcessContext: audio buffers via
    // context.input<float, N>() / context.output<float, N>(), events via context.inputEvents() and
    // context.outputEvents(), the block length via context.numFrames(), plus transport info.
    // If you ever need the raw CLAP struct, it's still there via context.native().
    //
    // This template renders a plain sine wave so you can hear something immediately. It ignores the
    // incoming MIDI; see the ExampleSineWaveSynth in libs/applause/examples for note-driven voices.
    applause::ProcessStatus process(applause::ProcessContext& context) noexcept override;

private:
    // These extensions hook directly into the host via CLAP ABI and provide essential functionality.
    // Your plugin won't do much without audio ports, for instance.
    // Applause is entirely modular, so you will have to manage these yourselves. Nothing is "baked in" to the library.
    // Use everything that you need, and nothing you don't.

    // Do you want MIDI input? You'll need a NotePortsExtension!
    applause::NotePortsExtension note_ports_;

    // Ditto for audio. If you want your plugin to make sound, you'll need to instantiate and configure one of these.
    applause::AudioPortsExtension audio_ports_;

    // The StateExtension allows you to save and restore plugin state, e.g. when opening/closing a daw project,
    // or copy and pasting plugin instances across tracks. Very useful!
    applause::StateExtension state_;

    // The Params Extension provides support for hosted parameters. Most plugins will want this.
    // UI parameter components, naturally, also expect a ParamsExtension to be present.
    applause::ParamsExtension params_;

    // If you want a GUI, use this extension. Pretty self-explanatory.
    applause::GUIExtension gui_ext_;

    // A cached handle to our frequency parameter. Handles are the audio-thread-friendly way to read
    // parameter values: they're lock-free and cheap. Look them up once (see the constructor) and hold
    // onto them; don't call getHandle() from process().
    applause::ParamHandle* frequency_param_ = nullptr;

    // The sample rate the host gave us in activate(). We need it to know how fast to advance the
    // oscillator: a 440 Hz tone advances through its cycle twice as fast at 44.1 kHz as it does at 88.2 kHz.
    double sample_rate_ = 44100.0;

    // How far through the sine wave we are, in radians. This has to persist across process() calls,
    // otherwise every block would restart the wave from zero and you'd hear clicks.
    double phase_ = 0.0;
};
