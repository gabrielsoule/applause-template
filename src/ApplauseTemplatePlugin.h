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

    // The main audio processing function. This is the heart of your plugin.
    // Here, you'll perform DSP, respond to events, and output audio samples and/or MIDI notes.
    // For now, all the relevant info you'll need (buffer pointers, MIDI events, etc) is inside the clap_process_t struct.
    // In the future, Applause will wrap this nicely for you, but for now, you can dig aroud the C struct yourself.
    clap_process_status process(const clap_process_t* process) noexcept override;

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
};
