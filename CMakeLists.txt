cmake_minimum_required(VERSION 3.22)
project(AUDIOCORE_DARK_BASS VERSION 0.1.0)

include(FetchContent)
FetchContent_Declare(juce GIT_REPOSITORY https://github.com/juce-framework/JUCE.git GIT_TAG 8.0.10 GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(juce)

juce_add_plugin(AUDIOCORE_DARK_BASS
    COMPANY_NAME "AUDIOCORE"
    BUNDLE_ID "radio.audiocore.darkbass"
    PLUGIN_MANUFACTURER_CODE AuCo
    PLUGIN_CODE DkBs
    FORMATS VST3 Standalone
    PRODUCT_NAME "AUDIOCORE DARK BASS"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
)

target_sources(AUDIOCORE_DARK_BASS PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h
)
target_compile_features(AUDIOCORE_DARK_BASS PRIVATE cxx_std_17)
target_compile_definitions(AUDIOCORE_DARK_BASS PUBLIC
    JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0 JUCE_VST3_CAN_REPLACE_VST2=0
)
target_link_libraries(AUDIOCORE_DARK_BASS PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
    PUBLIC juce::juce_recommended_config_flags
           juce::juce_recommended_lto_flags
           juce::juce_recommended_warning_flags
)
