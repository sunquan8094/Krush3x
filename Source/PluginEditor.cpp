/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Krush3xAudioProcessorEditor::Krush3xAudioProcessorEditor (Krush3xAudioProcessor& p, AudioProcessorValueTreeState &vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState(vts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    bitDepthAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "bitDepth", bitDepthController));
    bitDepthController.setTextValueSuffix( "Bits");
    bitDepthLabel.setText("Bit Depth", dontSendNotification);
    bitDepthLabel.attachToComponent(&bitDepthController, true);
    addAndMakeVisible(bitDepthLabel);
    addAndMakeVisible(bitDepthController);
  
    freqReductionAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "freqReduction", freqReductionController));
    freqReductionController.setTextValueSuffix(" Times");
    freqReductionLabel.setText("Frequency Reduction", dontSendNotification);
    freqReductionLabel.attachToComponent(&freqReductionController, true);
    addAndMakeVisible(freqReductionController);
    addAndMakeVisible(freqReductionLabel);
    
    freqReductionLabel.attachToComponent(&freqReductionController, true);
}

Krush3xAudioProcessorEditor::~Krush3xAudioProcessorEditor()
{
}

//==============================================================================
void Krush3xAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colours::white);
    g.setColour(Colours::black);
    g.setFont(18.0f);
}

void Krush3xAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

  bitDepthController.setBounds(50, 50, 300, 75);
  freqReductionController.setBounds(50, 100, 300, 75);
}
