#include <JuceHeader.h>
#include "PlaylistComponent.h"


//==============================================================================

PlaylistComponent::PlaylistComponent(DJAudioPlayer* _player1, DJAudioPlayer* _player2,
    AudioFormatManager& formatManagerToUse, WaveformDisplay* _waveformDisplay1,
    WaveformDisplay* _waveformDisplay2) :
    player1(_player1), player2(_player2),
    waveformDisplay1(_waveformDisplay1), waveformDisplay2(_waveformDisplay2)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    tableComponent.getHeader().addColumn("Playist",1,400);
    tableComponent.getHeader().addColumn("Player 1",2,100);
    tableComponent.getHeader().addColumn("Player 2",3,100);
    tableComponent.getHeader().addColumn("Option",4,100);

    tableComponent.setModel(this);

    addAndMakeVisible(tableComponent);
    addAndMakeVisible(searchingTextBox);

    searchingTextBox.setTextToShowWhenEmpty("Search for music", Colour::fromRGB(127, 82, 90));
    searchingTextBox.onReturnKey = [this] {searching(searchingTextBox.getText()); };
    tableComponent.setMultipleSelectionEnabled(true);

    getLookAndFeel().setColour(TextEditor::textColourId,Colour::fromRGB(255,204,153));
    getLookAndFeel().setColour(TextEditor::backgroundColourId,Colour::fromRGB(86,65,99));

    tableComponent.setColour(ListBox::backgroundColourId, Colour::fromRGB(218, 200, 245));
}

PlaylistComponent::~PlaylistComponent()
{

}

void PlaylistComponent::paint (juce::Graphics& g)
{
    // clear the background
    g.fillAll(Colour::fromRGB(193, 161, 202));

    // draw an outline around the component
    g.setColour (juce::Colours::orange);
    g.setFont (15.0f);
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    searchingTextBox.setBounds(0, 0, getWidth(), getHeight() / 9);
    tableComponent.setBounds(0, getHeight() / 9, getWidth(), 8* getHeight() /9);
}
int PlaylistComponent::getNumRows() {
    return trackTitlesNames.size();
}
void PlaylistComponent::paintRowBackground(Graphics& g, int rowNumber,
    int width, int height, bool rowIsSelected) 
{
    if (rowIsSelected) {
        g.fillAll(Colour::fromRGB(244, 164, 96));
    }
    else {
        g.fillAll(Colour::fromRGB(245, 222, 179));
    }
}

void PlaylistComponent::paintCell(Graphics& g, int rowNumber,
                                  int columnId, int width, int height,
                                  bool rowIsSelected) 
{
    g.drawText(trackTitlesNames[rowNumber],
        2,
        0,
        width - 4, 
        height, 
        Justification::centredLeft,
        true);
}

Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
                                                      int columnId, bool isRowSelected,
                                                       Component* existingComponentToUpdate) 
{
    if (columnId == 2) {
        if (existingComponentToUpdate == nullptr) {
            TextButton* btn = new TextButton{ "Deck 1" };
            String rowid{ std::to_string(rowNumber) };
            String colid{ std::to_string(columnId) };
            btn->setComponentID(colid + rowid);
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    if (columnId == 3) {
        if (existingComponentToUpdate == nullptr) {
            TextButton* btn = new TextButton{ "Deck 2" };
            String rowid{ std::to_string(rowNumber) };
            String colid{ std::to_string(columnId) };
            btn->setComponentID(colid + rowid);
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    if (columnId == 4) {
        if (existingComponentToUpdate == nullptr) {
            TextButton* btn = new TextButton{ "Remove" };
            String rowid{ std::to_string(rowNumber) };
            String colid{ std::to_string(columnId) };
            btn->setComponentID(colid + rowid);
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    return existingComponentToUpdate;

}

void PlaylistComponent::buttonClicked(Button* button) 
{
    if (true) 
    {
        std::string idStr = button->getComponentID().toStdString();
        int id = std::stoi(idStr);
        int trackIndex = std::stoi(idStr.substr(1));
   
        if (idStr.at(0) == '2') {
            player1->loadURL(URL{ trackTitles[trackIndex] });
            waveformDisplay1->loadURL(URL{ trackTitles[trackIndex] });
        }
        if (idStr.at(0) == '3') {
            player2->loadURL(URL{ trackTitles[trackIndex] });
            waveformDisplay2->loadURL(URL{ trackTitles[trackIndex] });
        }
        if (idStr.at(0) == '4') {
            if (trackIndex <= trackTitles.size() - 1 && trackIndex <= trackTitlesNames.size() - 1) {
                trackTitles.erase(trackTitles.begin() + trackIndex);
                trackTitlesNames.erase(trackTitlesNames.begin() + trackIndex);
                tableComponent.updateContent();
            }
        }
    }
    
}

bool PlaylistComponent::isInterestedInFileDrag(const StringArray& files) 
{
    return true;
}

void PlaylistComponent::filesDropped(const StringArray& files, int x, int y) {
    if (files.size() >= 1)
    {
        for (int i = 0; i < files.size(); i++) {
            trackTitlesNames.push_back(File{ files[i] } .getFileNameWithoutExtension());
            trackTitles.push_back(URL{ File{files[i]} });
            juce::String urljucet = URL{ File{files[i]} }.toString(false);
            std::string urlstdt = urljuce.toStdString();
        }
        tableComponent.updateContent();
    }
}

void PlaylistComponent::searching(String inputtext) 
{
    if (inputtext == "") {
        tableComponent.deselectAllRows();
    }
    else {
        for (int i = 0; i < trackTitlesNames.size(); i++) {
            if (trackTitlesNames[i].contains(inputtext)) {
                tableComponent.selectRow(i, false, false);
            }
        }
    }
}


