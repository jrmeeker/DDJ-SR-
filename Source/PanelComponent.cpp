#pragma once 

#include "PanelComponent.h"

PanelComponent::PanelComponent()
{
    //shift button 
    addAndMakeVisible(&shiftButton);
    shiftButton.setBounds(10, getHeight() - 255, 40, 20);
    shiftButton.setButtonText("SHIFT");
    shiftButton.onClick = [this] {shiftButtonClicked(); };

    //vinyl (jog mode) button
    addAndMakeVisible(&vinylButton);
    vinylButton.setBounds(10, 200, 50, 20);
    vinylButton.setButtonText("VINYL");
    vinylButton.onClick = [this] {vinylButtonClicked(); };
    vinylButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkslategrey);

    //generate path for shape buttons
    circle1.addEllipse(20, getHeight() - 100, 40, 40);

    //playButton
    addAndMakeVisible(&playButton);
    playButton.setShape(circle1, true, true, true);
    playButton.onClick = [this] {playButtonClicked(); };

    //cuebutton
    addAndMakeVisible(&cueButton);
    cueButton.setShape(circle1, true, true, true);
    cueButton.onClick = [this] {cueButtonClicked();  };

    //syncButton 
    addAndMakeVisible(&syncButton);
    syncButton.setShape(circle1, true, true, true);
    syncButton.onClick = [this] {syncButtonClicked(); };

    //deck buttons 
    addAndMakeVisible(&deckButton1);
    deckButton1.setShape(circle1, true, true, true);
    deckButton1.onClick = [this] {deckButton1Clicked(); };

    addAndMakeVisible(&deckButton3);
    deckButton3.setShape(circle1, true, true, true);
    deckButton3.onClick = [this] {deckButton3Clicked();  };

    //slip button 
    addAndMakeVisible(&slipButton);
    slipButton.setShape(circle1, true, true, true);
    slipButton.onClick = [this] {slipButtonClicked();  };

    //tap button 
    addAndMakeVisible(&tapButton);
    tapButton.setShape(circle1, true, true, true);
    tapButton.onClick = [this] {tapButtonClicked(); };

    //on button 
    addAndMakeVisible(&onButton);
    onButton.setShape(circle1, true, true, true);
    onButton.onClick = [this] {onButtonClicked(); };

    //auto loop button 
    addAndMakeVisible(&autoLoopButton);
    autoLoopButton.setButtonText("AUTO LOOP");
    autoLoopButton.onClick = [this] {autoLoopClicked(); };
    autoLoopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkslategrey);

    // 1/2X and 2X buttons
    addAndMakeVisible(&halfXButton);
    halfXButton.setButtonText("-");
    halfXButton.onClick = [this] {halfXButtonClicked(); };
    halfXButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkslategrey);

    addAndMakeVisible(&twoXButton);
    twoXButton.setButtonText("-");
    twoXButton.onClick = [this] {twoXButtonClicked(); };
    twoXButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkslategrey);

    //left and right parameter buttons 
    addAndMakeVisible(&rightButton);
    rightButton.setButtonText(">");
    rightButton.onClick = [this] {rightButtonClicked(); };
    rightButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkslategrey);

    addAndMakeVisible(&leftButton);
    leftButton.setButtonText("<");
    leftButton.onClick = [this] {leftButtonClicked(); };
    leftButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkslategrey);

    //tempo range button 
    addAndMakeVisible(&tempoRangeButton);
    tempoRangeButton.setBounds(350, 250, 40, 20);
    tempoRangeButton.setButtonText("-");
    tempoRangeButton.onClick = [this] { tempoRangeClicked(); };
    tempoRangeButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkslategrey);

    //tempo slider 
    addAndMakeVisible(tempoSlider);
    tempoSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    tempoSlider.setRange(MIN_TEMPO, MAX_TEMPO);
    tempoSlider.setValue(1.0);
    tempoSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    tempoSlider.addListener(this);

    //volume slider 
    /*addAndMakeVisible(volumeSlider);
    volumeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    volumeSlider.setRange(MIN_VOLDB, MAX_VOLDB);
    volumeSlider.setValue(-24.0);
    volumeSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    volumeSlider.addListener(this);*/

    //wheel slider
    addAndMakeVisible(wheelSlider);
    wheelSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    wheelSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    wheelSlider.setLookAndFeel(&lookFeel);
    wheelSlider.setRotaryParameters(0.f, 6.3f, false);
    wheelSlider.addListener(this);
    prevAngle = wheelSlider.getValue();

    //pad grid
    addAndMakeVisible(pads);

    //bottom button grid 
    addAndMakeVisible(bottomButtons);

    //fx1 buttons
    addAndMakeVisible(fx1Buttons);

    //FX knobs and buttons (top) 
    addAndMakeVisible(knobsAndButtons);

    //manage labels 
    addAndMakeVisible(yayLabel);
    yayLabel.setText("PLAY/PAUSE", juce::dontSendNotification);
    cueLabel.setJustificationType(juce::Justification::centredTop);
    yayLabel.attachToComponent(&playButton, false);

    addAndMakeVisible(cueLabel);
    cueLabel.setText("CUE", juce::dontSendNotification);
    cueLabel.setJustificationType(juce::Justification::centredTop);
    cueLabel.attachToComponent(&cueButton, false);

    addAndMakeVisible(syncLabel);
    syncLabel.setText("SYNC", juce::dontSendNotification);
    syncLabel.setJustificationType(juce::Justification::centredTop);
    syncLabel.attachToComponent(&syncButton, false);

    addAndMakeVisible(tempoLabel);
    tempoLabel.setText("TEMPO", juce::dontSendNotification);
    tempoLabel.attachToComponent(&tempoSlider, false);
    tempoSlider.addListener(this);

    //addAndMakeVisible(timeLabel);
    timeLabel.setBounds(217, 280, 80, 40);
    timeLabel.setText("00:00:000", juce::dontSendNotification);

    //primitives 
    tempo = 1.0;
    blockCount = 0;

    //resampling
    juce::ResamplingAudioSource resampler(&mixerSource, false, 2);
    stashSource = &resampler;

    //prepare main component
    formatManager.registerBasicFormats();
    transportSource.addChangeListener(this);

    //basic settings
    setAudioChannels(2, 2);
    setSize(1300, 700);

    //start timer for timerCallback()
    startTimer(17);
}


PanelComponent::~PanelComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

void PanelComponent::enablePlayButton()
{
    playButton.setEnabled(true);
}
///////////////////////////////////////////////////////////////////////////////////////
void PanelComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    stashSource->prepareToPlay(samplesPerBlockExpected, sampleRate);
    samplesPerBlock = samplesPerBlockExpected;
}

void PanelComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& info)
{
    if (readerSource.get() == nullptr)
    {
        info.clearActiveBufferRegion();
        return;
    }

    transportSource.getNextAudioBlock(info);
    currentPos = transportSource.getCurrentPosition();
    blockCount++;
}

void PanelComponent::releaseResources()
{
    mixerSource.releaseResources();
}

void PanelComponent::resized()
{
    shiftButton.setBounds(10, getHeight() - 285, 50, 20);
    playButton.setBounds(10, getHeight() - 80, 65, 65);
    cueButton.setBounds(10, getHeight() - 165, 65, 65);
    syncButton.setBounds(23, getHeight() - 230, 40, 40);
    deckButton1.setBounds(20, 275, 35, 35);
    deckButton3.setBounds(20, 335, 35, 35);
    slipButton.setBounds(380, 130, 35, 35);
    tapButton.setBounds(460, 360, 35, 35);
    onButton.setBounds(460, 420, 35, 35);
    tempoRangeButton.setBounds(460, 250, 40, 20);
    autoLoopButton.setBounds(430, 500, 70, 20);
    halfXButton.setBounds(430, 540, 25, 20);
    twoXButton.setBounds(475, 540, 25, 20);
    rightButton.setBounds(475, 650, 25, 20);
    leftButton.setBounds(430, 650, 25, 20);
    tempoSlider.setBounds(455, 30, 50, 140);
    //volumeSlider.setBounds(550, 400, 50, 140);
    wheelSlider.setBounds(90, 135, 335, 335);
    pads.setBounds(120, 540, 350, 140);
    bottomButtons.setBounds(120, 500, 300, 20);
    fx1Buttons.setBounds(20, 25, 80, 20);
    knobsAndButtons.setBounds(120, 20, 280, 150);
}

void PanelComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &transportSource)
    {
        if (transportSource.isPlaying())
            changeState(Playing);
        else if (state == Stopping || state == Playing)
            changeState(Stopped);
        else if (state == Pausing)
            changeState(Paused);
    }
}

void PanelComponent::changeState(TransportState newState)
{
    if (state != newState)
    {
        state = newState;
        switch (state)
        {
        case Stopped:
            transportSource.setPosition(0.0);
            break;

        case Starting:
            transportSource.start();
            break;

        case Playing:
            break;

        case Pausing:
            transportSource.stop();
            break;

        case Paused:
            break;

        case Stopping:
            transportSource.stop();
            break;
        }
    }
}

void PanelComponent::loadButtonClicked()
{
    chooser = std::make_unique<juce::FileChooser>("Select a Wave file to play...",
        juce::File{},
        "*.wav");
    auto chooserFlags = juce::FileBrowserComponent::openMode
        | juce::FileBrowserComponent::canSelectFiles;

    chooser->launchAsync(chooserFlags, [this](const juce::FileChooser& fc)
        {
            file = fc.getResult();

            if (file != juce::File{})
            {
                reader = formatManager.createReaderFor(file);

                if (reader != nullptr)
                {
                    newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, false);
                    transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
                    mixerSource.addInputSource(&transportSource, false);

                    playButton.setEnabled(true);
                    readerSource.reset(newSource.release());
                }
            }
        });
}

void PanelComponent::playButtonClicked()
{
    if ((state == Stopped) || (state == Paused))
        changeState(Starting);
    else if (state == Playing)
        changeState(Pausing);
}

void PanelComponent::cueButtonClicked()
{
    if (state == Paused)
        changeState(Stopped);
    else
        changeState(Stopping);
}

void PanelComponent::syncButtonClicked()
{}

void PanelComponent::shiftButtonClicked()
{}

void PanelComponent::deckButton1Clicked()
{}

void PanelComponent::deckButton3Clicked()
{}

void PanelComponent::vinylButtonClicked()
{}

void PanelComponent::slipButtonClicked()
{}

void PanelComponent::tapButtonClicked()
{}

void PanelComponent::onButtonClicked()
{}

void PanelComponent::tempoRangeClicked()
{}

void PanelComponent::autoLoopClicked()
{}

void PanelComponent::halfXButtonClicked()
{}

void PanelComponent::twoXButtonClicked()
{}

void PanelComponent::leftButtonClicked()
{}

void PanelComponent::rightButtonClicked()
{}

void PanelComponent::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &tempoSlider)
    {
        tempo = slider->getValue();
        if (tempo != 1.0)
        {
            //update display 
            juce::String rsString = std::to_string(tempo);
            ratioLabel.setText(rsString, juce::dontSendNotification);

            //store current track info
            juce::int64 pos = transportSource.getNextReadPosition();
            prevLength = transportSource.getTotalLength();

            //release audio source
            transportSource.stop();
            mixerSource.releaseResources();

            //reload source
            newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, false);
            transportSource.setSource(newSource.get(), 0, nullptr, tempo);

            //resample source
            juce::ResamplingAudioSource resampler(&transportSource, false, 2);
            stashSource = &resampler;
            stashSource->setResamplingRatio(tempo);
            stashSource->prepareToPlay(samplesPerBlock, tempo);

            //release reader
            readerSource.reset(newSource.release());

            //get new track data
            double newLen = transportSource.getLengthInSeconds();
            juce::int64 newTotalLen = transportSource.getTotalLength();

            //establish new read pointer position
            double ratio = (double)newTotalLen / prevLength;
            newPos = (juce::int64)(ratio * pos);

            //set read pointer and restart playback
            transportSource.setNextReadPosition(newPos);
            transportSource.start();
        }
    }

    /*if (slider == &volumeSlider)
    {
        volDB = slider->getValue();

        if (volDB < -47)
            volume = 0;
        else
            volume = pow(10, volDB / 20);

        transportSource.setGain(volume);
    }*/

    if (slider == &wheelSlider)
    {
        wheelEngaged = true;
        transportSource.stop();

        wheelAngle = slider->getValue();
        juce::String rsString = std::to_string(wheelAngle);
        wheelLabel.setText(rsString, juce::dontSendNotification);

        double deltaAngle = wheelAngle - prevAngle;

        //handle special case of crossing from 10.0 to 0.0 or vice-versa
        if (prevAngle > 9 && wheelAngle < 0.2)
            deltaAngle = wheelAngle;
        else if (prevAngle < 1 && wheelAngle > 9)
            deltaAngle = -prevAngle;

        double dLength = transportSource.getLengthInSeconds();
        dNewPos = currentPos + (deltaAngle * dLength / 100);

        if (dNewPos < 0.0)
            dNewPos = 0.0;

        transportSource.setPosition(dNewPos);
        currentPos = dNewPos;
        prevAngle = wheelAngle;

    }
}

void PanelComponent::sliderDragEnded(juce::Slider* slider)
{
    if (wheelEngaged)
    {
        transportSource.start();
    }
    wheelEngaged = false;
}


void PanelComponent::timerCallback()
{
    juce::RelativeTime position(transportSource.getCurrentPosition());

    auto minutes = ((int)position.inMinutes()) % 60;
    auto seconds = ((int)position.inSeconds()) % 60;
    auto millis = ((int)position.inMilliseconds()) % 1000;

    auto positionString = juce::String::formatted("%02d:%02d:%03d", minutes, seconds, millis);
    timeLabel.setText(positionString, juce::dontSendNotification);
}

//==============================================================================






//==============================================================================
void PanelComponent::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::whitesmoke);
    g.drawRoundedRectangle(16, 270, 44, 105, 5, 1); //deck buttons
    g.drawRoundedRectangle(15, 10, 85, 55, 5, 1);
    g.drawRoundedRectangle(456, 355, 44, 105, 5, 1);
}