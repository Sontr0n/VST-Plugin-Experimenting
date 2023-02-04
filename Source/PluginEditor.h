/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include <D:/Projects/JUCE/Tutorial/Source/synth_audio_source.cpp>
#include <D:/Projects/JUCE/Tutorial/Source/PluginEditor.h>




//==============================================================================
/**
*/

class TutorialAudioProcessorEditor  : public juce::AudioProcessorEditor, 
                                      private juce::Slider::Listener,
                                      public juce::Timer,
                                      public juce::AudioAppComponent,
                                    
                                      public juce::Component
                                                               
{
public:
    TutorialAudioProcessorEditor (TutorialAudioProcessor&);
    ~TutorialAudioProcessorEditor() override;

    void setMidiInput(int index);
    void timerCallback() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void AudioAppComponent::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill);

private:
  void sliderValueChanged (juce::Slider* slider) override;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TutorialAudioProcessor& audioProcessor;
    
    juce::Slider midiVolume;
    
    juce::MidiKeyboardState keyboardState;
    
    SynthAudioSource synthAudioSource;
    juce::MidiKeyboardComponent keyboardComponent;

    juce::AudioDeviceManager deviceManager;
    juce::ComboBox midiInputList;
    juce::Label midiInputListLabel;
    int lastInputIndex = 0;

    

    
        
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TutorialAudioProcessorEditor)
};
