#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

class Krush3xAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    Krush3xAudioProcessorEditor (Krush3xAudioProcessor&, AudioProcessorValueTreeState&);
    ~Krush3xAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    Krush3xAudioProcessor& processor;
  
    AudioProcessorValueTreeState& valueTreeState;
  
    Slider bitDepthController;
    Label bitDepthLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bitDepthAttachment;
  
    Slider freqReductionController;
    Label freqReductionLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> freqReductionAttachment;
  
    Slider dryWetController;
    Label dryWetLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dryWetAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Krush3xAudioProcessorEditor)
};
