#include "PluginEditor.h"
AudioCoreDarkBassAudioProcessorEditor::AudioCoreDarkBassAudioProcessorEditor(AudioCoreDarkBassAudioProcessor& p)
: AudioProcessorEditor(&p), processor(p)
{
    setSize(620, 360);
    title.setText("AUDIOCORE", juce::dontSendNotification); title.setFont(juce::Font(25.f, juce::Font::bold)); title.setColour(juce::Label::textColourId, juce::Colours::white); addAndMakeVisible(title);
    subtitle.setText("DARK BASS  /  LOW-END PROCESSOR", juce::dontSendNotification); subtitle.setFont(juce::Font(12.f, juce::Font::bold)); subtitle.setColour(juce::Label::textColourId, juce::Colour(0xffff3038)); addAndMakeVisible(subtitle);
    setup(bass, "BASS"); setup(sub, "SUB FREQ"); setup(drive, "DRIVE"); setup(mix, "MIX"); setup(output, "OUTPUT");
    bassA = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "BASS", bass);
    subA = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "SUB", sub);
    driveA = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "DRIVE", drive);
    mixA = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "MIX", mix);
    outputA = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "OUTPUT", output);
}
void AudioCoreDarkBassAudioProcessorEditor::setup(Slider& s, const juce::String& name)
{ s.setSliderStyle(Slider::RotaryHorizontalVerticalDrag); s.setTextBoxStyle(Slider::TextBoxBelow, false, 76, 20); s.setName(name); s.setColour(Slider::rotarySliderFillColourId, juce::Colour(0xffff3038)); s.setColour(Slider::textBoxTextColourId, juce::Colours::white); s.setColour(Slider::textBoxOutlineColourId, juce::Colours::darkgrey); addAndMakeVisible(s); }
void AudioCoreDarkBassAudioProcessorEditor::paint(juce::Graphics& g)
{ g.fillAll(juce::Colour(0xff101114)); g.setColour(juce::Colour(0xff292b30)); g.fillRoundedRectangle(14.f, 92.f, 592.f, 245.f, 10.f); g.setColour(juce::Colour(0xffff3038)); g.fillRect(14, 82, 592, 3); g.setColour(juce::Colours::grey); g.setFont(11.f); g.drawText("AUDIOCORE TOOLS  •  VST3", 420, 24, 180, 20, juce::Justification::centredRight); }
void AudioCoreDarkBassAudioProcessorEditor::resized()
{ title.setBounds(22, 14, 250, 36); subtitle.setBounds(24, 48, 300, 22); Slider* all[] = { &bass, &sub, &drive, &mix, &output }; for (int i=0;i<5;++i) all[i]->setBounds(25 + i*118, 115, 108, 190); }
