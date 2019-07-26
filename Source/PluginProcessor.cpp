#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>

Krush3xAudioProcessor::Krush3xAudioProcessor()
     : AudioProcessor (BusesProperties()
                       .withInput  ("Input",  AudioChannelSet::stereo())
                       .withOutput ("Output", AudioChannelSet::stereo())), parameters(new AudioProcessorValueTreeState(*this, nullptr, "PARAMETERS", {
       std::make_unique<AudioParameterInt>("bitDepth", "Bit Depth", 2, 24, 24),
       std::make_unique<AudioParameterFloat>("freqReduction", "Frequency Reduction", NormalisableRange<float>(0.1f, 1.0f, 0.1f), 1.0f),
       std::make_unique<AudioParameterFloat>("dryWet", "Dry Wet Mix", NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f)
     }))
{
    phase = 0.0f;
}

Krush3xAudioProcessor::~Krush3xAudioProcessor()
{
}

//==============================================================================
const String Krush3xAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Krush3xAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Krush3xAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Krush3xAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Krush3xAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Krush3xAudioProcessor::getNumPrograms()
{
    return 1;
}

int Krush3xAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Krush3xAudioProcessor::setCurrentProgram (int index)
{
}

const String Krush3xAudioProcessor::getProgramName (int index)
{
    return {};
}

void Krush3xAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Krush3xAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{

}

void Krush3xAudioProcessor::releaseResources()
{
  
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Krush3xAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Krush3xAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
  
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel) {
      float* channelData = buffer.getWritePointer (channel), bitDepthValue = (float)*parameters->getRawParameterValue(StringRef("bitDepth")), dryWetVal = (float)*parameters->getRawParameterValue(StringRef("dryWet"));
      for (int i = 0; i < buffer.getNumSamples(); i++) {
        float twoPower = exp2(bitDepthValue) - 1,
        rawNum = round((channelData[i] + 1) / 2 * twoPower) / twoPower * 2 - 1,
        wetSig = rawNum * dryWetVal, drySig = channelData[i] * (1.0f - dryWetVal);
          phase += (float)*parameters->getRawParameterValue(StringRef("freqReduction"));
          if (phase >= 1.0f) {
            phase -= 1.0f;
          }
          channelData[i] = wetSig + drySig;
      }
    }
}

//==============================================================================
bool Krush3xAudioProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* Krush3xAudioProcessor::createEditor()
{
    return new Krush3xAudioProcessorEditor (*this, *parameters);
}

//==============================================================================
void Krush3xAudioProcessor::getStateInformation (MemoryBlock& destData)
{
  std::unique_ptr<XmlElement> xml (new XmlElement ("Krush3x"));
  xml->setAttribute ("bitDepth", (int)*(parameters->getRawParameterValue(StringRef("bitDepth"))));
  xml->setAttribute ("freqReduction", (double) *(parameters->getRawParameterValue(StringRef("freqReduction"))));
  xml->setAttribute ("dryWet", (double) *(parameters->getRawParameterValue(StringRef("dryWet"))));
  copyXmlToBinary (*xml, destData);
}

void Krush3xAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
  std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
  if (xmlState.get() != nullptr)
  {
    if (xmlState->hasTagName ("Krush3x"))
    {
      Value bitDepth = parameters->getParameterAsValue("bitDepth"), freqReduction = parameters->getParameterAsValue("freqReduction"), dryWet = parameters->getParameterAsValue("dryWet");
      bitDepth = (int) xmlState->getIntAttribute ("bitDepth", 24);
      freqReduction = (float) xmlState->getDoubleAttribute ("freqReduction", 1.0);
      dryWet = (float) xmlState->getDoubleAttribute ("dryWet", 0.5f);
    }
  }
}

AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Krush3xAudioProcessor();
}
