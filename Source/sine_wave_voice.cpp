#include <JuceHeader.h>
#include <D:/Projects/JUCE/Tutorial/Source/sine_wave_sound.cpp>

class SineWaveVoice : public juce::SynthesiserVoice {
public: 
	bool canPlaySound(juce::SynthesiserSound* sound) override {
		return dynamic_cast<SineWaveSound*> (sound) != nullptr;
	}

	void startNote(int midiNoteNumber,
		float velocity,
		juce::SynthesiserSound*,
		int /*currentPitchWheelPosition*/) override {
		currentAngle = 0.0;
		level = velocity * .15;
		tailOff = 0.0;

		auto cyclesPerSecond = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
		auto cyclesPerSample = cyclesPerSecond / getSampleRate();

		angleDelta = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;
	}

	void renderNextBlock(juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override {
		if (angleDelta != 0) {
			if (tailOff > 0) {
				while (--numSamples >= 0) {
					auto currentSample = (float)(std::sin(currentAngle) * level * tailOff);

					for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
						outputBuffer.addSample(i, startSample, currentSample);
					
					currentAngle += angleDelta;
					++startSample;

					tailOff *= .99;

					if (tailOff <= .005) {
						clearCurrentNote();
						
						angleDelta = 0.0;
						break;
					}
					
				}
			}
			else {
				while (--numSamples >= 0) {
					auto currentSample = (float)(std::sin(currentAngle) * level);
					
					for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
						outputBuffer.addSample(i, startSample, currentSample);

					currentAngle += angleDelta;
					++startSample;
				}
			}
		}
	}

	void stopNote(float /*velocity*/, bool allowTailOff) override {
		if (allowTailOff) {
			if (tailOff == 0.0)
				tailOff = 1.0;
		}
		else {
			clearCurrentNote();
			angleDelta = 0.0;
		}
	}

	void pitchWheelMoved(int newPitchWheelValue ) override{}
	void controllerMoved(int controllerNumber, int newControllerValue) override {}
private: 
	double currentAngle = 0.0, angleDelta = 0.0, level = 0.0, tailOff = 0.0, attack = 0.0;
};