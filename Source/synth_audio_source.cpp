#include <JuceHeader.h>
#include <juce_audio_basics/synthesisers/juce_Synthesiser.h>

class SynthAudioSource : public juce::AudioSource, public juce::MidiKeyboardState{
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
    }

    void releaseResources() override {};

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override {
        bufferToFill.clearActiveBufferRegion();
        juce::MidiBuffer incomingMidi;
        keyboardState.processNextMidiBuffer(incomingMidi, bufferToFill.startSample, bufferToFill.numSamples, true);

        synth.renderNextBlock(*bufferToFill.buffer, incomingMidi, bufferToFill.startSample, bufferToFill.numSamples);
    }

private:

    juce::MidiKeyboardState& keyboardState;
    juce::Synthesiser synth;
    
};