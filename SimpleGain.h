#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <JuceHeader.h>
#include "PlugInGUISettings.h"
#include "SynchronBlockProcessor.h"
#include "SmoothParameter.h"
class SimpleGainParameter
{
public:
	void addParameter(std::vector < std::unique_ptr<RangedAudioParameter>>& paramVector);

public:
    std::atomic<float>* m_gain;
    float m_gainOld;
};



class SimpleGain : public SynchronBlockProcessor
{
public:
    SimpleGain();
    void prepareToPlay(float samplerate);
    // void processBlock(juce::AudioBuffer<float>&);
    int  processSynchronBlock(std::vector <std::vector<float>>& data, juce::MidiBuffer& midiMessages);
    void setGain(float newgain_db);

    void prepareParameter(std::unique_ptr<AudioProcessorValueTreeState>&  vts);

private:
    float m_gaindB;
    float m_gain;
    SmoothParameterLinear m_gainSmoothed;
    // parameter handling
    SimpleGainParameter m_gainparameter;
    void updateParameter()
    {
        if (hasparameterChanged(*m_gainparameter.m_gain, m_gainparameter.m_gainOld))
	    {
            m_gaindB = m_gainparameter.m_gainOld;
		    setGain(m_gaindB);
        }
    }
    bool hasparameterChanged(float valNew, float &valOld)
    {
        if (valOld != valNew)
        {
            valOld = valNew;
            return true;
        }
        else
        {
            return false;
        }
    };    
};

const struct
{
	const std::string ID = "Gain";
	std::string name = "Gain";
	std::string unitName = "dB";
	float minValue = -90.f;
	float maxValue = 20.f;
	float defaultValue = 0.f;
}paramGain;

class SimpleGainComponent : public Component
{
public:
	SimpleGainComponent(AudioProcessorValueTreeState& );

	void paint(Graphics& g) override;
	void resized() override;
    std::function<void()> somethingChanged;
    
    void setScaleFactor(float newscale){m_scaleFactor = newscale;};
private:
    float m_scaleFactor;
    AudioProcessorValueTreeState& m_vts; 

    Label m_GainLabel;
    Slider m_GainSlider;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> m_GainAttachment;
};

