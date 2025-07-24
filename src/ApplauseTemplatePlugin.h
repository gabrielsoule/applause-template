#pragma once

#include "core/PluginBase.h"
#include "extensions/AudioPortsExtension.h"
#include "extensions/NotePortsExtension.h"
#include "extensions/StateExtension.h"
#include "extensions/ParamsExtension.h"
#include "extensions/GUIExtension.h"
#include "ApplauseTemplateEditor.h"
#include "util/DebugHelpers.h"

class ApplauseTemplatePlugin : public applause::PluginBase {
public:
    ApplauseTemplatePlugin(const clap_plugin_descriptor_t* descriptor, const clap_host_t* host);
    ~ApplauseTemplatePlugin() override = default;
    
    // Called when an instance of your plugin is instantiated by the host
    bool init() noexcept override;

    // Called when the host deletes your plugin instance
    void destroy() noexcept override;

    // Called when a sample rate and buffer size is established; similar to Juce's prepareToPlay
    // Can be called many times over a plugin instance's lifetime
    bool activate(double sampleRate, uint32_t minFrameCount, uint32_t maxFrameCount) noexcept override;

    // Called when the plugin is deactivated and no longer expected to process audio, but not deleted
    void deactivate() noexcept override;
    
    // The main audio processing function. This is the heart of your plugin.
    // Here, you'll perform DSP, respond to events, and output audio samples and/or MIDI notes.
    clap_process_status process(const clap_process_t* process) noexcept override;
    
private:
    // These extensions hook directly into the CLAP ABI and provide essential functionality.
    // Your plugin won't do much without audio ports, for instance.
    // Applause is entirely modular, so you will have to manage these yourselves. Nothing is "baked in" to the library.
    // Use everything that you need, and nothing you don't.
    applause::NotePortsExtension note_ports_;
    applause::AudioPortsExtension audio_ports_;
    applause::StateExtension state_;
    applause::ParamsExtension params_;
    applause::GUIExtension<ApplauseExampleEditor> gui_ext_;
};