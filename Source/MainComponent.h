#pragma once

#include <JuceHeader.h>
#include <limits>

#define MIN_TEMPO 0.75f
#define MAX_TEMPO 1.25f
#define MAX_VOLDB 0
#define MIN_VOLDB -48
#define MAX_ANGLE 20.0f
#define MIN_ANGLE -20.0f

class LookFeel : public juce::LookAndFeel_V4
{
public:
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
        float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override
    {
        float diameter = juce::jmin(width, height);
        float radius = diameter / 2;
        float centerX = x + width / 2;
        float centerY = y + height / 2;
        float rx = centerX - radius;
        float ry = centerY - radius;
        float angle = rotaryStartAngle + (sliderPos * (rotaryEndAngle - rotaryStartAngle));

        juce::Rectangle<float> dialArea(rx, ry, diameter, diameter);
        g.setColour(juce::Colours::silver);
        g.fillEllipse(dialArea);     

        g.setColour(juce::Colours::black);
        g.fillEllipse(centerX - 60, centerY - 60, 120, 120);

        juce::Path wheelTick;
        wheelTick.addRectangle(0, -radius, 2.0f, radius);

        g.setColour(juce::Colours::black);
        g.fillPath(wheelTick, juce::AffineTransform::rotation(angle).translated(centerX, centerY));
    }
};

class KnobLookFeel : public juce::LookAndFeel_V4
{
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
        float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override
    {
        float diameter = juce::jmin(width, height);
        float radius = diameter / 2;
        float centerX = x + width / 2;
        float centerY = y + height / 2;
        float rx = centerX - radius;
        float ry = centerY - radius;
        float angle = rotaryStartAngle + (sliderPos * (rotaryEndAngle - rotaryStartAngle));

        juce::Rectangle<float> dialArea(rx, ry, diameter, diameter);
        g.setColour(juce::Colours::black);
        g.fillEllipse(dialArea);

        juce::Path wheelTick;
        g.setColour(juce::Colours::white);
        wheelTick.addRectangle(0, -radius, 2.0f, radius);

        g.setColour(juce::Colours::white);
        g.fillPath(wheelTick, juce::AffineTransform::rotation(angle).translated(centerX, centerY));
    }
};

struct ControlColumn : public juce::Component
{
    KnobLookFeel knobLookFeel;
    ControlColumn()
    {
        for (int i = 0; i < 5; ++i)
        {
            addAndMakeVisible(knobs.add(new juce::Slider));
            knobs[i]->setSliderStyle(juce::Slider::Rotary);
            knobs[i]->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        }
    }

    void resized() override
    {
        auto bounds = juce::Rectangle<int>(500, 0, 100, 300);
        auto knobWidth = bounds.getWidth() / 3;
        auto knobHeight = knobWidth;

        for (int i = 0; i < 5; ++i)
        {
            knobs[i]->setBounds(getWidth() / 3, 65 * i, knobWidth, knobHeight);
            knobs[i]->setLookAndFeel(&knobLookFeel);
        }
    }
    
    juce::OwnedArray<juce::Slider> knobs;
};

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent  : public juce::AudioAppComponent,
                              public juce::ChangeListener,
                              public juce::Slider::Listener,
                              public juce::Timer

{
public:
    enum TransportState
    {
        Stopped,
        Starting,
        Playing,
        Pausing,
        Paused,
        Stopping
    };
    
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;
        //==============================================================================

    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    void changeState(TransportState newState);
    void loadButtonClicked();
    void shiftButtonClicked();
    void playButtonClicked();
    void cueButtonClicked();
    void syncButtonClicked();
    void deckButton1Clicked();
    void deckButton3Clicked();
    void vinylButtonClicked();
    void slipButtonClicked();
    void tapButtonClicked();
    void onButtonClicked();
    void tempoRangeClicked();
    void autoLoopClicked();
    void halfXButtonClicked();
    void twoXButtonClicked();
    void leftButtonClicked();
    void rightButtonClicked();

    void sliderValueChanged(juce::Slider* slider) override;

    //taken from AudioTransportSource
    //bool hasStreamFinished() const noexcept;

    void sliderDragEnded(juce::Slider* slider) override;
    void timerCallback() override;




private:
    //==============================================================================
    // Your private member variables go here...
    
    //buttons
    juce::TextButton loadButton;
    juce::TextButton shiftButton;
    juce::TextButton vinylButton;
    juce::TextButton tempoRangeButton;
    juce::TextButton autoLoopButton;
    juce::TextButton halfXButton;
    juce::TextButton twoXButton;
    juce::TextButton rightButton;
    juce::TextButton leftButton;

    juce::ShapeButton playButton{ "Play", juce::Colours::silver, juce::Colours::silver, juce::Colours::darkgreen };
    juce::ShapeButton cueButton{ "Cue", juce::Colours::silver, juce::Colours::silver, juce::Colours::darkgreen };
    juce::ShapeButton syncButton{ "Sync", juce::Colours::silver, juce::Colours::silver, juce::Colours::darkred };
    juce::ShapeButton deckButton1{ "1", juce::Colours::slategrey, juce::Colours::slategrey, juce::Colours::darkred };
    juce::ShapeButton deckButton3{ "3", juce::Colours::slategrey, juce::Colours::slategrey, juce::Colours::darkred };
    juce::ShapeButton slipButton{ "Slip", juce::Colours::slategrey, juce::Colours::slategrey, juce::Colours::darkred };
    juce::ShapeButton tapButton{ "Tap", juce::Colours::slategrey, juce::Colours::slategrey, juce::Colours::darkred }; 
    juce::ShapeButton onButton{ "On", juce::Colours::slategrey, juce::Colours::slategrey, juce::Colours::darkred };

    //structs 
    ControlColumn leftControlColumn;

    //pads 
    struct PadPanel : public juce::Component
    {
        int NUM_PADS = 8;

        PadPanel()
        {
            for (int i = 0; i < NUM_PADS; ++i)
            {
                addAndMakeVisible(pads.add(new juce::TextButton));
                pads[i]->setColour(juce::TextButton::buttonColourId, juce::Colours::beige);
            }
        }

        void resized() override
        {
            auto bounds = juce::Rectangle<int>(100, 480, 340, 140);
            auto padSize = bounds.getWidth() / 4 - 23;

            pads[0]->setBounds(0, 0, padSize, padSize);
            pads[4]->setBounds(0, padSize + 10, padSize, padSize);

            for (int i = 1; i < NUM_PADS / 2; ++i)
            {
                pads[i]->setBounds((padSize + 10) * i, 0, padSize, padSize);
            }

            for (int i = 5; i < NUM_PADS; ++i)
            {
                pads[i]->setBounds((padSize + 10) * (i-4), padSize + 10, padSize, padSize);
            }
        }

        //juce::Colour backgroundColor;
        juce::OwnedArray<juce::TextButton> pads;
    }; 
    PadPanel pads; //instantiate

    //bottom buttons
    struct BottomButtonPanel : public juce::Component
    {
        int NUM_BUTTONS = 4;
        BottomButtonPanel()
        {
            for (int i = 0; i < NUM_BUTTONS; ++i)
            {
                addAndMakeVisible(buttons.add(new juce::TextButton));
                buttons[i]->setColour(juce::TextButton::buttonColourId, juce::Colours::darkslategrey);
            }

            buttons[0]->setButtonText("HOT CUE");
            buttons[1]->setButtonText("ROLL");
            buttons[2]->setButtonText("SLICER");
            buttons[3]->setButtonText("SAMPLER");
        }

        void resized() override
        {
            auto bounds = juce::Rectangle<int>(100, 480, 340, 20);
            auto buttonWidth = bounds.getWidth() / 4 - 23;
            auto buttonHeight = bounds.getHeight();

            buttons[0]->setBounds(0, 0, buttonWidth, buttonHeight);
            for (int i = 1; i < NUM_BUTTONS; ++i)
            {
                buttons[i]->setBounds((buttonWidth + 10) * i, 0, buttonWidth, buttonHeight);
            }
        }

        juce::OwnedArray<juce::TextButton> buttons;
    };
    BottomButtonPanel bottomButtons;

    //FX1 assign buttons
    struct FXButtonPanel : public juce::Component
    {
        FXButtonPanel()
        {
            for (int i = 0; i < 2; ++i)
            {
                addAndMakeVisible(buttons.add(new juce::TextButton));
                buttons[i]->setColour(juce::TextButton::buttonColourId, juce::Colours::darkslategrey);
            }
            buttons[0]->setButtonText("1");
            buttons[1]->setButtonText("2");
        }

        void resized() override
        {
            auto bounds = juce::Rectangle<int>(20, 20, 80, 20);
            auto buttonWidth = 35;
            auto buttonHeight = bounds.getHeight();

            buttons[0]->setBounds(0, 0, buttonWidth, buttonHeight);
            buttons[1]->setBounds(40, 0, buttonWidth, buttonHeight);
        }

        juce::OwnedArray < juce::TextButton> buttons;
    };
    FXButtonPanel fx1Buttons;

    //Top FX knobs & buttons
    struct FXKnobsAndButtons : public juce::Component
    {
        KnobLookFeel knobLookFeel;
        FXKnobsAndButtons()
        {
            for (int i = 0; i < 4; ++i)
            {
                addAndMakeVisible(knobs.add(new juce::Slider));
                knobs[i]->setSliderStyle(juce::Slider::Rotary);
                knobs[i]->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
                //set look & feel for knob type

                addAndMakeVisible(buttons.add(new juce::TextButton));
                buttons[i]->setColour(juce::TextButton::buttonColourId, juce::Colours::darkslategrey);
            }
        }

        void resized() override
        {
            auto bounds = juce::Rectangle<int>(100, 0, 340, 150);
            auto knobWidth = bounds.getWidth() / 4 - 50;
            auto knobHeight = bounds.getHeight() / 2;
            
            for (int i = 0; i < 4; ++i)
            {
                knobs[i]->setBounds(15 + 71 * i, -15, knobWidth, knobHeight);
                knobs[i]->setLookAndFeel(&knobLookFeel);
            }
            
            auto buttonWidth = bounds.getWidth() / 4 - 23;
            auto buttonHeight = 20;

            for (int i = 0; i < 4; ++i)
            {
                buttons[i]->setBounds((buttonWidth + 10) * i, bounds.getHeight() - 80, buttonWidth, buttonHeight);
                buttons[i]->setButtonText("ON");
            }
            buttons[3]->setButtonText("TAP");
        }

        juce::OwnedArray<juce::Slider> knobs;
        juce::OwnedArray<juce::TextButton> buttons;
    };
    FXKnobsAndButtons knobsAndButtons;


    //sliders
    juce::Slider tempoSlider;
    juce::Slider volumeSlider;
    juce::Slider wheelSlider;

    //look and feel 
    LookFeel lookFeel;
    KnobLookFeel knobLookFeel;

    //Paths
    juce::Path circle1;

    //labels
    juce::Label yayLabel;
    juce::Label cueLabel;
    juce::Label syncLabel;
    juce::Label tempoLabel;
    juce::Label volumeLabel;
    juce::Label ratioLabel;
    juce::Label wheelLabel;
    juce::Label deltaLabel;
    juce::Label timeLabel;

    //file access
    std::unique_ptr<juce::FileChooser> chooser;

    //file format handling
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    TransportState state;

    juce::File file{};
    juce::AudioFormatReader* reader;
    std::unique_ptr<juce::AudioFormatReaderSource> newSource;
    juce::MixerAudioSource mixerSource;
    juce::AudioSourcePlayer sourcePlayer;
    juce::ResamplingAudioSource* stashSource;
    
    int samplesPerBlock;
    juce::int64 prevLength;
    juce::int64 newPos;
    juce::int64 blockCount;

    double tempo;
    double volDB;
    double volume;
    double wheelAngle;
    double prevAngle;
    double currentPos;
    double dNewPos;


    bool wheelEngaged;

    ///////////////////////////////////////////////////////////////////////
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};
