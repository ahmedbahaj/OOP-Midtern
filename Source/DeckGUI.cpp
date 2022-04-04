
#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player, 
                WaveformDisplay* _waveformDisplay) : player(_player), waveformDisplay(_waveformDisplay)
{
    // Buttons
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);

    addAndMakeVisible(loadButton);
    addAndMakeVisible(rewindButton);
    addAndMakeVisible(forwardButton);

    
    // Sliders   
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);

    addAndMakeVisible(volumn);
    addAndMakeVisible(position);
    addAndMakeVisible(speed);

    volSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    speedSlider.setSliderStyle(Slider::SliderStyle::Rotary);

    volSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 25);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 25);

    speedSlider.setValue(1.0);
    volSlider.setValue(1.0);


    getLookAndFeel().setColour(Slider::thumbColourId, Colour::fromRGB(65, 105, 225));
    getLookAndFeel().setColour(Slider::rotarySliderFillColourId, Colour::fromRGB(186, 85, 211));
    getLookAndFeel().setColour(TextButton::buttonColourId, Colour::fromRGB(135, 206, 250));

    getLookAndFeel().setColour(TextButton::textColourOffId, Colour::fromRGB(74, 0, 130));

    getLookAndFeel().setColour(Slider::trackColourId, Colour::fromRGB(209, 42, 86));

    volumn.setText("Volume", juce::dontSendNotification);
    position.setText("Position", juce::dontSendNotification);
    speed.setText("Speed", juce::dontSendNotification);

    volumn.setJustificationType(Justification::centredBottom);
    speed.setJustificationType(Justification::centredBottom);

    volumn.attachToComponent(&volSlider, false);
    position.attachToComponent(&posSlider, true);
    speed.attachToComponent(&speedSlider, false);


    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    rewindButton.addListener(this);
    forwardButton.addListener(this);

    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);


    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0, 10.0);
    posSlider.setRange(0.0, 1.0);

    startTimer(500);


}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (Graphics& g)
{
    g.fillAll(Colour::fromRGB(60, 73, 98));   // clear the background

    g.setColour (Colour::fromRGB(147, 112, 219));
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::snow);
    g.setFont (14.0f);
}

void DeckGUI::resized()
{
    auto labelposition = 55;//the sapce for the label
    double rowH = getHeight() / 8.0;
    double columnW = getWidth() / 3.0;
    playButton.setBounds(0, 0, getWidth(), rowH - 5);
    stopButton.setBounds(0, rowH - 5, getWidth(), rowH - 5); 
    loadButton.setBounds(columnW, rowH * 2 - 10, columnW, rowH - 5);
    rewindButton.setBounds(0, rowH * 2 - 10, columnW, rowH - 5);
    forwardButton.setBounds(columnW * 2, rowH * 2 - 10, columnW, rowH - 5);

    volSlider.setBounds(0, rowH * 2+40, getWidth() / 2, rowH * 4);
    speedSlider.setBounds(getWidth()/2, rowH * 2+40, getWidth()/2, rowH * 4);
    posSlider.setBounds(labelposition, rowH * 7, getWidth() - 10 - labelposition, rowH);
}

void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playButton)
    {
        player->start();
    }
     if (button == &stopButton)
    {
        player->stop();

    }

    if (button == &loadButton)
    {
        FileChooser chooser{"Select a file..."};
        if (chooser.browseForFileToOpen())
        {
            player->loadURL(URL{chooser.getResult()});
            waveformDisplay->loadURL(URL{chooser.getResult()});
            
        }
    }

    if (button == &rewindButton)
    {
        player->rewind();
    }

    if (button == &forwardButton)
    {
        player->forward();
    }
}

void DeckGUI::sliderValueChanged (Slider *slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }
    
    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }
    
}

bool DeckGUI::isInterestedInFileDrag (const StringArray &files)
{
  return true; 
}

void DeckGUI::filesDropped (const StringArray &files, int x, int y)
{
  if (files.size() == 1)
  {
    player->loadURL(URL{File{files[0]}});
    waveformDisplay->loadURL(URL{ File{files[0]} });
  }
}

void DeckGUI::timerCallback()
{
    waveformDisplay->setPositionRelative(player->getPositionRelative());

    if (player->getPositionRelative() >= 0.0 && player->getPositionRelative() <= 1.0)
    {
        posSlider.setValue(player->getPositionRelative());
    }
}


    

