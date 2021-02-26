#pragma once

#include "PluginProcessor.h"
#include "JadeLookAndFeel.h"
#include "PresetHandler.h"

#include "SimpleGain.h"

//==============================================================================
class SimpleGainAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    
    explicit SimpleGainAudioProcessorEditor (SimpleGainAudioProcessor&);
    ~SimpleGainAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JadeLookAndFeel m_jadeLAF;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SimpleGainAudioProcessor& m_processorRef;
    PresetComponent m_presetGUI;
#if WITH_MIDIKEYBOARD    
    MidiKeyboardComponent m_keyboard;
#endif
    // plugin specific components
    SimpleGainComponent m_gaincomponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleGainAudioProcessorEditor)
};
