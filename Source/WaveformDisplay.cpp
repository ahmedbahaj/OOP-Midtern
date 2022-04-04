
#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(AudioFormatManager & formatManagerToUse,
    AudioThumbnailCache & 	cacheToUse) :
    audioThumb(1000, formatManagerToUse, cacheToUse), 
    fileLoaded(false), 
    position(0)
                          
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

  audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (Graphics& g)
{
    g.fillAll(Colour::fromRGB(60, 73, 98));

    g.setColour(Colour::fromRGB(147, 112, 219));
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::hotpink);
    if(fileLoaded)
    {
      audioThumb.drawChannel(g, 
        getLocalBounds(), 
        0, 
        audioThumb.getTotalLength(), 
        0, 
        1.0f
      );
      g.setColour(Colours::snow);
      g.drawRect(position * getWidth(), 0, getWidth() / 50, getHeight());
    }
    else 
    {
    // Placeholder text
    g.setFont (20.0f);
    g.drawText ("File not loaded...", getLocalBounds(),
                Justification::centred, true);  
    }
}

void WaveformDisplay::resized()
{

}

void WaveformDisplay::loadURL(URL audioURL)
{
  audioThumb.clear();
  fileLoaded  = audioThumb.setSource(new URLInputSource(audioURL));
  if (fileLoaded)
  {
    repaint();
  }
  else 
  {

  }

}

void WaveformDisplay::changeListenerCallback (ChangeBroadcaster *source)
{
    repaint();
}

void WaveformDisplay::setPositionRelative(double pos)
{
  if (pos != position)
  {
    position = pos;
    repaint();
  }
}




