/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>

//==============================================================================
Krush3xAudioProcessor::Krush3xAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ), parameters(*this, nullptr)
#endif
{
    parameters.createAndAddParameter("bitDepth", "Bit Depth", String(), NormalisableRange<float>(2.0f, 24.0f, 1.0f), 24, nullptr, nullptr);
    parameters.state = ValueTree(Identifier("Krush3x"));
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
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
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
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void Krush3xAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Krush3xAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

float Krush3xAudioProcessor::rms(float* array, int length) {
  float sum = 0.0f;
  for (int i = 0; i < length; i++) {
    sum += array[i] * array[i];
  }
  return sqrt((1/length) * (sum));
}

void Krush3xAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    bitDepthParam = parameters.getRawParameterValue("bitDepth");
  
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.

    for (int channel = 0; channel < totalNumInputChannels; ++channel) {
      float sum = 0, avg = 0;
      float* channelData = buffer.getWritePointer (channel);
      for (int i = 0; i < buffer.getNumSamples(); i++) {
          float twoPowerLessOne = exp2(*bitDepthParam) - 1.0f, rawNum = floor(channelData[i] * twoPowerLessOne);
          channelData[i] = (float)(rawNum / twoPowerLessOne);
          sum += channelData[i];
      }
      avg = sum / buffer.getNumSamples();
      for (int i = 0; i < buffer.getNumSamples(); i++) {
        channelData[i] -= avg;
      }
    }
}

//==============================================================================
bool Krush3xAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Krush3xAudioProcessor::createEditor()
{
    return new Krush3xAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void Krush3xAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Krush3xAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Krush3xAudioProcessor();
}
