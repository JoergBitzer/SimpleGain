#include "SimpleGain.h"

SimpleGain::SimpleGain()
:m_gain(1.f)
{

}
void SimpleGain::prepareToPlay()
{
    
}
void SimpleGain::processBlock(juce::AudioBuffer<float>& buffer)
{
    updateParameter();
    
    auto totalNumChannels  = buffer.getNumChannels();

    auto nrofsamplesinblock = buffer.getNumSamples();

    for (int channel = 0; channel < totalNumChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        for (auto kk = 0 ; kk < nrofsamplesinblock; ++kk)
            channelData[kk] *= m_gain;
    }
}
void SimpleGain::setGain(float newgain_db)
{
    m_gaindB = newgain_db;
    m_gain = pow(10.f,m_gaindB/20.f);
}
void SimpleGain::prepareParameter(std::unique_ptr<AudioProcessorValueTreeState>&  vts)
{
    m_gainparameter.m_gain = vts->getRawParameterValue(paramGain.ID);
	m_gainparameter.m_gainOld = paramGain.defaultValue;
}




void SimpleGainParameter::addParameter(std::vector < std::unique_ptr<RangedAudioParameter>>& paramVector)
{
    paramVector.push_back(std::make_unique<AudioParameterFloat>(paramGain.ID,
        paramGain.name,
        NormalisableRange<float>(paramGain.minValue, paramGain.maxValue),
        paramGain.defaultValue,
        paramGain.unitName,
        AudioProcessorParameter::genericParameter,
        [](float value, int MaxLen) { return (String(int((value) * 2 + 0.5) * 0.5, MaxLen) ); },
        [](const String& text) {return text.getFloatValue(); }));
}


SimpleGainComponent::SimpleGainComponent(AudioProcessorValueTreeState& vts)
:somethingChanged(nullptr),m_vts(vts),m_scaleFactor(1.f)
{
	m_GainLabel.setText("Gain", NotificationType::dontSendNotification);
	m_GainLabel.setJustificationType(Justification::centred);
	m_GainLabel.attachToComponent (&m_GainSlider, false);
	addAndMakeVisible(m_GainLabel);

	m_GainSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	// m_GainSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 60, 20);
	m_GainAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(m_vts, paramGain.ID, m_GainSlider);
	addAndMakeVisible(m_GainSlider);
	m_GainSlider.onValueChange = [this]() {if (somethingChanged != nullptr) somethingChanged(); };

}

void SimpleGainComponent::paint(Graphics& g)
{
	g.fillAll((getLookAndFeel().findColour(ResizableWindow::backgroundColourId)).darker(0.2));

}
void SimpleGainComponent::resized()
{
	float scaleFactor = m_scaleFactor;

	auto r = getLocalBounds();

	// reduce for a small border
	int newBorderWidth = scaleFactor*GLOBAL_MIN_DISTANCE;
	r.reduce(newBorderWidth, newBorderWidth);
	auto s = r;
	auto t = r;
   	t = s.removeFromBottom(scaleFactor*(GLOBAL_MIN_LABEL_HEIGHT/2+GLOBAL_MIN_ROTARY_WIDTH));

	m_GainSlider.setBounds(t.removeFromLeft(scaleFactor*GLOBAL_MIN_ROTARY_WIDTH));
	m_GainSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true,scaleFactor* GLOBAL_MIN_ROTARY_TB_WIDTH, scaleFactor*GLOBAL_MIN_ROTARY_TB_HEIGHT);
	t.removeFromLeft(scaleFactor*GLOBAL_MIN_DISTANCE);	

}



