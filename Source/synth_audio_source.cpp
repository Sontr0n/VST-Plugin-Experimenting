#include <JuceHeader.h>
#include <juce_audio_basics/synthesisers/juce_Synthesiser.h>
#include <D:/Projects/JUCE/Tutorial/Source/sine_wave_sound.cpp>
#include <D:/Projects/JUCE/Tutorial/Source/sine_wave_voice.cpp>
#include <D:/Projects/JUCE/Tutorial/Source/PluginEditor.h>


class SynthAudioSource : public juce::AudioSource{
public:
    
    SynthAudioSource(juce::MidiKeyboardState& keyState)
        : keyboardState(keyState) {
        for (int i = 0; i < 17; ++i)
            synth.addVoice(new SineWaveVoice());
        synth.addSound(new SineWaveSound());
    }

    void setUsingSineWaveSound() {
        synth.clearSounds();
    }

    void prepareToPlay(int /*samplesPerBlockExpected*/, double sampleRate) override {
        synth.setCurrentPlaybackSampleRate(sampleRate);
        midiCollector.reset(sampleRate);
    }

    void releaseResources() override {}

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override {
        bufferToFill.clearActiveBufferRegion();
        
        juce::MidiBuffer incomingMidi;
        midiCollector.removeNextBlockOfMessages(incomingMidi, bufferToFill.numSamples);

        keyboardState.processNextMidiBuffer(incomingMidi, bufferToFill.startSample, bufferToFill.numSamples, true);

        synth.renderNextBlock(*bufferToFill.buffer, incomingMidi, bufferToFill.startSample, bufferToFill.numSamples);
    }

    juce::MidiMessageCollector* getMidiCollector() {
        return &midiCollector;
    }

private:
    juce::MidiKeyboardState& keyboardState;
    juce::Synthesiser synth;
    juce::MidiMessageCollector midiCollector;
    
};