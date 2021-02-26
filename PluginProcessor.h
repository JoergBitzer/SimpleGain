#pragma once
#include <vector>
#include <juce_audio_processors/juce_audio_processors.h>
#include "PresetHandler.h"

#include "SimpleGain.h"

//==============================================================================
class SimpleGainAudioProcessor  : public juce::AudioProcessor
{
public:
    friend class SimpleGainAudioProcessorEditor;
    //==============================================================================
    SimpleGainAudioProcessor();
    ~SimpleGainAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    CriticalSection m_protect;
    float m_fs; // sampling rate is always needed

    //Parameterhandling
    std::unique_ptr<AudioProcessorValueTreeState> m_parameterVTS;
    std::vector <std::unique_ptr<RangedAudioParameter>> m_paramVector;
	PresetHandler m_presets;
#if WITH_MIDIKEYBOARD    
    MidiKeyboardState m_keyboardState;
#endif
    // your plugin stuff
    SimpleGain m_simplegain;
    SimpleGainParameter m_simplegainparameter;

    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleGainAudioProcessor)
};
