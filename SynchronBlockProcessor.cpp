#include "SynchronBlockProcessor.h"

SynchronBlockProcessor::SynchronBlockProcessor()
:m_NrOfChannels(2),m_OutBlockSize(256),m_maxInputSize(256)
{
    preparetoProcess(m_NrOfChannels,m_maxInputSize);
    
}
void SynchronBlockProcessor::preparetoProcess(int channels, int maxinputlen)
{
    m_NrOfChannels = channels;
    m_maxInputSize = maxinputlen;
    m_mem.resize(m_NrOfChannels);
    m_block.resize(m_NrOfChannels);
    for (auto kk = 0; kk < m_NrOfChannels; ++kk)
    {
        m_mem.at(kk).resize(2*m_OutBlockSize);
        m_block.at(kk).resize(m_OutBlockSize);
        std::fill(m_mem.begin(),m_mem.end(),0.f);
        std::fill(m_block.begin(),m_block.end(),0.f);
    }
    m_OutCounter = 0;
    m_InCounter = 0;
}
int SynchronBlockProcessor::processBlock(std::vector <std::vector<float>>& m_data)
{
    int nrOfInputSamples = m_data[0].size();
    for (auto kk = 0; kk < nrOfInputSamples; ++kk)
    {
        for (auto cc = 0; cc < m_NrOfChannels; ++cc)
        {
            m_block[cc][m_InCounter] = m_data[cc][kk];
            m_data[cc][kk] = m_mem[cc][m_OutCounter];
        }
        m_InCounter++;
        if (m_InCounter == m_OutBlockSize)
        {
            m_InCounter = 0;
            processSynchronBlock(m_block);
                // copy block into mem
            if (m_OutCounter < m_OutBlockSize)
            {
                for (auto channel = 0; channel < m_NrOfChannels; ++channel)
                {
                    for (auto sample = 0; sample < m_OutBlockSize; ++sample)
                    {
                        m_mem[channel][sample + m_OutBlockSize] = m_block[channel][sample];
                    }
                }
 
            }
            else
            {
                for (auto channel = 0; channel < m_NrOfChannels; ++channel)
                {
                    for (auto sample = 0; sample < m_OutBlockSize; ++sample)
                    {
                        m_mem[channel][sample] = m_block[channel][sample];
                    }
                }
            }
            
        }
        m_OutCounter++;
        if (m_OutCounter == 2*m_OutBlockSize)
            m_OutCounter = 0;
    }
}

int SynchronBlockProcessor::setDesiredBlockSizeSamples(int desiredSize)
{
    m_OutBlockSize = desiredSize;
    preparetoProcess(m_NrOfChannels,m_maxInputSize);

}
int SynchronBlockProcessor::setDesiredBlockSizeMiliSeconds(float desiredSize_ms, float fs)
{
    int desiredSize = int(desiredSize_ms*0.001*fs + 0.5);
    setDesiredBlockSizeSamples(desiredSize);
}

int SynchronBlockProcessor::getDelay()
{
    return m_OutBlockSize;
}