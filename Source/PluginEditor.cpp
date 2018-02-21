/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Krush3xAudioProcessorEditor::Krush3xAudioProcessorEditor (Krush3xAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    bitDepthController.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    bitDepthController.setRange(2, 24, 1);
    bitDepthController.setValue(24);
    bitDepthController.setTextValueSuffix("Bits");
    addAndMakeVisible(bitDepthController);
    bitDepthController.addListener(this);
  
    addAndMakeVisible(&bitDepthLabel);
    bitDepthLabel.setText("Bit Depth", dontSendNotification);
    bitDepthLabel.attachToComponent(&bitDepthController, true);
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
  
    //g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void Krush3xAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
      
    bitDepthController.setBounds(50, 50, 300, 75);
}

void Krush3xAudioProcessorEditor::sliderValueChanged(Slider* slider) {
    if (slider == &bitDepthController)
      processor.bitDepth = bitDepthController.getValue();
}
