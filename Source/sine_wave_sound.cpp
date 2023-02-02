#include <JuceHeader.h>

struct SineWaveSound : public juce::SynthesizerSound {
    SineWaveSound(){};

    bool appliesToNote (int) override {return true;}
    bool appliesToChannel(int) override {return true;}


};