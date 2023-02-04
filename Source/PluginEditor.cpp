/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <JuceHeader.h>
#include <D:/Projects/JUCE/Tutorial/Source/sine_wave_sound.cpp>
#include <D:/Projects/JUCE/Tutorial/Source/sine_wave_voice.cpp>
#include <D:/Projects/JUCE/Tutorial/Source/synth_audio_source.cpp>


//==============================================================================
TutorialAudioProcessorEditor::TutorialAudioProcessorEditor (TutorialAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), synthAudioSource (keyboardState),
      keyboardComponent(keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard) 
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    juce::Component::setSize (600, 160);

    //add the listener to the slider
    midiVolume.addListener (this);

    //these define the parameters of our slider object 
    midiVolume.setSliderStyle(juce::Slider::LinearBarVertical);
    midiVolume.setRange(0.0, 127.0, 1.0);
    midiVolume.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    midiVolume.setPopupDisplayEnabled(true, false, this);
    midiVolume.setTextValueSuffix("Volume");
    midiVolume.setValue(1.0);

    //this function adds the slider to the editor
    juce::Component::addAndMakeVisible(&midiVolume);
    
    juce::Component::addAndMakeVisible(keyboardComponent);
    setAudioChannels(0, 2);
    startTimer(400);

    juce::Component::addAndMakeVisible(midiInputListLabel);
    midiInputListLabel.setText("MIDI Input:", juce::dontSendNotification);
    midiInputListLabel.attachToComponent(&midiInputList, true);

    auto midiInputs = juce::MidiInput::getAvailableDevices();
    juce::Component::addAndMakeVisible(midiInputList);
    midiInputList.setTextWhenNoChoicesAvailable("No MIDI Inputs Enabled");

    juce::StringArray midiInputNames;
    for (auto input : midiInputs)
        midiInputNames.add(input.name);

    midiInputList.addItemList(midiInputNames, 1);
    midiInputList.onChange = [this] {setMidiInput(midiInputList.getSelectedItemIndex()); };

    for (auto input : midiInputs) {
        if (deviceManager.isMidiInputDeviceEnabled(input.identifier)) {
            setMidiInput(midiInputs.indexOf(input));
            break;
        }
    }

    if (midiInputList.getSelectedId() == 0)
        setMidiInput(0);
}

void TutorialAudioProcessorEditor::setMidiInput(int index) {
    auto list = juce::MidiInput::getAvailableDevices();

    deviceManager.removeMidiInputDeviceCallback(list[lastInputIndex].identifier,
                                                synthAudioSource.getMidiCollector());

    auto newInput = list[index];

    if (!deviceManager.isMidiInputDeviceEnabled(newInput.identifier))
        deviceManager.setMidiInputDeviceEnabled(newInput.identifier, true);

    deviceManager.addMidiInputDeviceCallback(newInput.identifier, synthAudioSource.getMidiCollector());
    midiInputList.setSelectedId(index + 1, juce::dontSendNotification);

    lastInputIndex = index;
}


void TutorialAudioProcessorEditor::timerCallback()  
{
    keyboardComponent.grabKeyboardFocus();
    stopTimer();
}

TutorialAudioProcessorEditor::~TutorialAudioProcessorEditor()
{
}

void TutorialAudioProcessorEditor::sliderValueChanged (juce::Slider* slider) {
    audioProcessor.noteOnVel = midiVolume.getValue();
}

//==============================================================================
void TutorialAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Component::getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Midi Volume", juce::Component::getLocalBounds(), juce::Justification::centred, 1);
}

void TutorialAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    midiInputList.setBounds(200, 10, juce::Component::getWidth() - 210, 20);
    keyboardComponent.setBounds(10, 40, juce::Component::getWidth() - 20, juce::Component::getHeight() - 50);

    midiVolume.setBounds(40, 30, 20, juce::Component::getHeight() - 60);
    
    setResizable(true, true);
}
