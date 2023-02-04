#include <JuceHeader.h>


struct SineWaveSound : public juce::SynthesiserSound {
    SineWaveSound(){};

    bool appliesToNote (int) override {return true;}
    bool appliesToChannel(int) override {return true;}


};