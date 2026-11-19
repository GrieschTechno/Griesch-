#include "PluginProcessor.h"
#include "PluginEditor.h"

AudioCoreDarkBassAudioProcessor::AudioCoreDarkBassAudioProcessor()
: AudioProcessor(BusesProperties().withInput("Input", juce::AudioChannelSet::stereo(), true)
                                  .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
  parameters(*this, nullptr, "PARAMETERS", createParameterLayout()) {}

AudioCoreDarkBassAudioProcessor::APVTS::ParameterLayout AudioCoreDarkBassAudioProcessor::createParameterLayout()
{
    APVTS::ParameterLayout p;
    p.add(std::make_unique<juce::AudioParameterFloat>("BASS", "Bass", juce::NormalisableRange<float>(-12.f, 12.f, .1f), 0.f));
    p.add(std::make_unique<juce::AudioParameterFloat>("SUB", "Sub", juce::NormalisableRange<float>(20.f, 120.f, 1.f), 35.f));
    p.add(std::make_unique<juce::AudioParameterFloat>("DRIVE", "Drive", juce::NormalisableRange<float>(0.f, 100.f, 1.f), 0.f));
    p.add(std::make_unique<juce::AudioParameterFloat>("MIX", "Mix", juce::NormalisableRange<float>(0.f, 100.f, 1.f), 100.f));
    p.add(std::make_unique<juce::AudioParameterFloat>("OUTPUT", "Output", juce::NormalisableRange<float>(-18.f, 6.f, .1f), 0.f));
    return p;
}

void AudioCoreDarkBassAudioProcessor::prepareToPlay(double sr, int block)
{
    spec = { sr, (juce::uint32) block, (juce::uint32) getTotalNumOutputChannels() };
    dryBuffer.setSize(getTotalNumOutputChannels(), block, false, false, true);
    lowShelfL.prepare(spec); lowShelfR.prepare(spec); subL.prepare(spec); subR.prepare(spec);
    lowShelfL.reset(); lowShelfR.reset(); subL.reset(); subR.reset();
}
void AudioCoreDarkBassAudioProcessor::releaseResources() {}
bool AudioCoreDarkBassAudioProcessor::isBusesLayoutSupported(const BusesLayout& l) const
{ return l.getMainInputChannelSet() == juce::AudioChannelSet::stereo() && l.getMainOutputChannelSet() == juce::AudioChannelSet::stereo(); }

void AudioCoreDarkBassAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    const int n = buffer.getNumSamples();
    if (buffer.getNumChannels() < 2 || dryBuffer.getNumSamples() < n) return;
    dryBuffer.makeCopyOf(buffer, true);
    auto* left = buffer.getWritePointer(0);
    auto* right = buffer.getWritePointer(1);
    const float bass = parameters.getRawParameterValue("BASS")->load();
    const float subHz = parameters.getRawParameterValue("SUB")->load();
    const float drive = parameters.getRawParameterValue("DRIVE")->load() * .08f;
    const float mix = parameters.getRawParameterValue("MIX")->load() * .01f;
    const float out = juce::Decibels::decibelsToGain(parameters.getRawParameterValue("OUTPUT")->load());
    auto shelf = juce::dsp::IIR::Coefficients<float>::makeLowShelf(spec.sampleRate, 110.0, .707f, juce::Decibels::decibelsToGain(bass));
    auto sub = juce::dsp::IIR::Coefficients<float>::makeLowPass(spec.sampleRate, subHz);
    *lowShelfL.coefficients = *shelf; *lowShelfR.coefficients = *shelf;
    *subL.coefficients = *sub; *subR.coefficients = *sub;
    for (int i = 0; i < n; ++i)
    {
        float l = lowShelfL.processSample(left[i]);
        float r = lowShelfR.processSample(right[i]);
        const float sl = subL.processSample(l), srSub = subR.processSample(r);
        const float monoSub = (sl + srSub) * .5f;
        l = l * .75f + monoSub * .25f;
        r = r * .75f + monoSub * .25f;
        if (drive > 0.0001f)
        {
            const float gain = 1.0f + drive;
            l = std::tanh(l * gain) / std::tanh(gain);
            r = std::tanh(r * gain) / std::tanh(gain);
        }
        left[i] = (dryBuffer.getSample(0, i) * (1.f - mix) + l * mix) * out;
        right[i] = (dryBuffer.getSample(1, i) * (1.f - mix) + r * mix) * out;
    }
}

juce::AudioProcessorEditor* AudioCoreDarkBassAudioProcessor::createEditor() { return new AudioCoreDarkBassAudioProcessorEditor(*this); }
void AudioCoreDarkBassAudioProcessor::getStateInformation(juce::MemoryBlock& d) { auto s = parameters.copyState(); std::unique_ptr<juce::XmlElement> x(s.createXml()); copyXmlToBinary(*x, d); }
void AudioCoreDarkBassAudioProcessor::setStateInformation(const void* data, int size) { std::unique_ptr<juce::XmlElement> x(getXmlFromBinary(data, size)); if (x && x->hasTagName(parameters.state.getType())) parameters.replaceState(juce::ValueTree::fromXml(*x)); }
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new AudioCoreDarkBassAudioProcessor(); }
