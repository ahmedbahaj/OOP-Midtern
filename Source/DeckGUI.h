 
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "DeckGUI.h"

//==============================================================================
/*
*/
class DeckGUI    : public Component,
                   public Button::Listener, 
                   public Slider::Listener, 
                   public FileDragAndDropTarget, 
                   public Timer
{
public:
    DeckGUI(DJAudioPlayer* player,
            WaveformDisplay* waveformDisplay);
    ~DeckGUI();

    void paint (Graphics&) override;
    void resized() override;

     // implement button's listener
    void buttonClicked (Button *) override;

    // implement slider's listener
    void sliderValueChanged (Slider *slider) override;

    // File drop
    bool isInterestedInFileDrag (const StringArray &files) override;
    void filesDropped (const StringArray &files, int x, int y) override; 

    void timerCallback() override; 

private:

    TextButton playButton{ "PLAY" };
    TextButton stopButton{ "STOP" };

    TextButton loadButton{ "LOAD" };
    TextButton rewindButton{ "<<" };
    TextButton forwardButton{ ">>" };
    
    Slider volSlider; 
    juce::Label volumn;

    Slider speedSlider;
    juce::Label speed;

    Slider posSlider;
    juce::Label position;

    // Constructors
    WaveformDisplay* waveformDisplay;
    DJAudioPlayer* player; 

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
