cmake_minimum_required(VERSION 3.21)
project(ApplauseTemplate VERSION 0.1.0 LANGUAGES C CXX)

# Enable OBJC/OBJCXX on macOS
if (APPLE)
    enable_language(OBJC)
    enable_language(OBJCXX)
endif ()

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Control whether Applause builds its examples
set(APPLAUSE_BUILD_EXAMPLES ON CACHE BOOL "Build Applause examples" FORCE)

# Add the Applause library from submodule
add_subdirectory(libs/applause EXCLUDE_FROM_ALL)

# Auto-discover source files
file(GLOB_RECURSE PLUGIN_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

# The add_applause_plugin utility automatically generates the CLAP plugin entry .cpp glue code that
# you would otherwise have to write by hand.

# When a CLAP plugin is scanned, the host calls functions in the plugin entry .cpp file to learn
# about the plugin, rather than loading and unloading the plugin itself.
# This significantly accelerates the scanning phase, compared to, e.g., VST3.

# Congruent with our design philosophy, the add_applause_plugin utility is entirely optional.
# The ExampleManualPluginEntry in the Applause repo shows how to make a plugin entry .cpp file and build
# an applause plugin the "old fashioned" way. This may be useful if you want to invoke custom behavior during
# the scanning phase, such as a licensing check.
add_applause_plugin(
        TARGET_NAME ${PROJECT_NAME}
        OUTPUT_NAME "ApplauseTemplate"
        PLUGIN_CLASS ApplauseTemplatePlugin
        PLUGIN_HEADER "ApplauseTemplatePlugin.h"

        # Plugin descriptor - CUSTOMIZE THESE!
        PLUGIN_ID "com.yourcompany.yourplugin"
        PLUGIN_NAME "Your Plugin Name"
        PLUGIN_VENDOR "Your Company"
        PLUGIN_URL "https://yourwebsite.com"
        PLUGIN_VERSION ${PROJECT_VERSION}
        PLUGIN_DESCRIPTION "Description of your plugin"
        # Search for plugin-features.h in the CLAP source repository for a complete list of CLAP feature flags
        PLUGIN_FEATURES INSTRUMENT SYNTHESIZER

        SOURCES ${PLUGIN_SOURCES}

        # Plugin formats to generate
        PLUGIN_FORMATS CLAP VST3 AUV2

        # Bundle information - CUSTOMIZE THESE!
        BUNDLE_IDENTIFIER "com.yourcompany.yourplugin"
        BUNDLE_VERSION ${PROJECT_VERSION}

        # AUV2 specific - CUSTOMIZE THESE!
        # Manufacturer name must be exactly 3 characters
        AUV2_MANUFACTURER_NAME "YCO"
        # Manufacturer code must be exactly 4 characters
        AUV2_MANUFACTURER_CODE "Yco1"
        # Subtype code must be exactly 4 characters
        AUV2_SUBTYPE_CODE "YPl1"
        # Instrument type (aumu for music device, aufx for effect)
        AUV2_INSTRUMENT_TYPE "aumu"

        # Choose whether to copy the built plugins into your user plugin directory. Helpful for testing!
        COPY_AFTER_BUILD TRUE
)