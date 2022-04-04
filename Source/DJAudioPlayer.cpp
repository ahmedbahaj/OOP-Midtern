#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(AudioFormatManager& _formatManager) 
: formatManager(_formatManager)
{

}
DJAudioPlayer::~DJAudioPlayer()
{

}

void DJAudioPlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate) 
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}
void DJAudioPlayer::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    resampleSource.getNextAudioBlock(bufferToFill);

}
void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

void DJAudioPlayer::loadURL(URL audioURL)
{
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) // good file!
    {       
        std::unique_ptr<AudioFormatReaderSource> newSource (new AudioFormatReaderSource (reader, true)); 

        juce::String total_path = audioURL.getDomain() + "/" + audioURL.getSubPath();
        transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);             
        readerSource.reset (newSource.release());          
    }
}
void DJAudioPlayer::setGain(double gain)
{
    if (gain < 0 || gain > 1.0)
    {
        std::cout << "gain should be between 0 and 1" << std::endl;
    }
    else {
        transportSource.setGain(gain);
    }
   
}

void DJAudioPlayer::setSpeed(double ratio)
{
  if (ratio < 0 || ratio > 100.0)
    {
        std::cout << "ratio should be between 0 and 100" << std::endl;
    }
    else {
        resampleSource.setResamplingRatio(ratio);
    }
}

void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos)
{
     if (pos < 0 || pos > 1.0)
    {
        std::cout << "pos should be between 0 and 1" << std::endl;
    }
    else {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}


void DJAudioPlayer::start()
{
    transportSource.start();
}
void DJAudioPlayer::stop()
{
    transportSource.stop();
}

void DJAudioPlayer::resume() {
    setPosition(0.0);
}

double DJAudioPlayer::getPositionRelative()
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

void DJAudioPlayer::rewind()
{
    // if after move is less than 0 or not for short audios
    if (transportSource.getCurrentPosition() - 1 > 0)
    {
        transportSource.setPosition(transportSource.getCurrentPosition() - 1.5);
    }
}

//Moving the playback forward
void DJAudioPlayer::forward()
{
    //Calculating the last position on the transportSource - end of the track
    double last_pos{ transportSource.getLengthInSeconds() };

    // This prevents moving if the audio is almost finished
    if (transportSource.getCurrentPosition() + 0.5 != last_pos || transportSource.getCurrentPosition() + 0.5 > last_pos)
    {
        transportSource.setPosition(transportSource.getCurrentPosition() + 1.5);
    }
}
