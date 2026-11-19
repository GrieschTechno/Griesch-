#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class AudioCoreDarkBassAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit AudioCoreDarkBassAudioProcessorEditor(AudioCoreDarkBassAudioProcessor&);
    void paint(juce::Graphics&) override;
    void resized() override;
private:
    using Slider = juce::Slider;
    AudioCoreDarkBassAudioProcessor& processor;
    Slider bass, sub, drive, mix, output;
    juce::Label title, subtitle;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bassA, subA, driveA, mixA, outputA;
    void setup(Slider&, const juce::String&);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioCoreDarkBassAudioProcessorEditor)
};
