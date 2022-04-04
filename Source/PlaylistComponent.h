#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include "DeckGUI.h"
#include "WaveformDisplay.h"
#include <fstream>

//==============================================================================
class PlaylistComponent  : public juce::Component,
                           public TableListBoxModel,
                           public Button::Listener,
                           public FileDragAndDropTarget
                           
{
public:
    PlaylistComponent(DJAudioPlayer* _player1, DJAudioPlayer* _player2,
        AudioFormatManager& formatManagerToUse, 
        WaveformDisplay* _waveformDisplay1,
        WaveformDisplay* _waveformDisplay2);
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    int getNumRows() override;
    void paintRowBackground(Graphics&, int rowNumber,
        int width, int height, bool rowIsSelected) override;

    void paintCell(Graphics&, int rowNumber,
        int columnId, int width, int height,
        bool rowIsSelected) override;

    Component* refreshComponentForCell(int rowNumber,
        int columnId, bool isRowSelected,
        Component* existingComponentToUpdate) override;

    void buttonClicked(Button* button) override;

    bool isInterestedInFileDrag(const StringArray& files) override;
    void filesDropped(const StringArray& files, int x, int y) override;

    void loadURL(URL audioURL);
    std::vector<std::string> tokenise(std::string Line, char separator);

private:
    juce::TableListBox tableComponent;
    juce::TextEditor searchingTextBox;

    // File's name
    std::vector<juce::String> trackTitlesNames;
    
    // File's URL
    std::vector<URL> trackTitles;

    juce::String urljuce;
    std::string urlstd;
    std::string filenamestd;
    std::ofstream playlistSave;

    DJAudioPlayer* player1;
    DJAudioPlayer* player2;
    WaveformDisplay* waveformDisplay1;
    WaveformDisplay* waveformDisplay2;

    SafePointer<DialogWindow> dialogWindow;

    void searching(String inputtext);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};

