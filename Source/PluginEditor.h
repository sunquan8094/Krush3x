/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class Krush3xAudioProcessorEditor  : public AudioProcessorEditor, private Slider::Listener
{
public:
    Krush3xAudioProcessorEditor (Krush3xAudioProcessor&);
    ~Krush3xAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    void sliderValueChanged(Slider* slider) override;
  
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Krush3xAudioProcessor& processor;
  
    Slider bitDepthController;
    Label bitDepthLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Krush3xAudioProcessorEditor)
};
