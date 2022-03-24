//
//  Composer.h
//  dvoinoi-artea
//
//  Created by Alexander Bogdanov
//  Copyright © 2022 dvoinoi. All rights reserved.
//

#include <JuceHeader.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include "Composer.h"
#include "Bootstrap.h"
#include "ParameterPoll.h"

// some global constants
int MAX_POLYPHONY = 100;
int AUDIO_INPUT_CHANNELS = 2; // will be 8
int AUDIO_OUTPUT_CHANNELS = 2;

void samplerLoadNewSample(Synthesiser* sampler, std::string fileName, int assignToMidiNoteNumber, double attackTime = 0., double releaseTime = 0., double maxSampleLength = 10.)
{
    // TODO move to Composer
    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();
    juce::AudioFormatReader* audioReader = formatManager.createReaderFor(juce::File(juce::String(fileName)));
    
    BigInteger sampleMidiRange;
    sampleMidiRange.setRange(assignToMidiNoteNumber, assignToMidiNoteNumber + 1, true); // NOTE, im not checking bounds here TODO add max and min check
    
    sampler->addSound(new SamplerSound(fileName,                 // this is a "name", since we don't care about names, we can just use the filename
                                      *audioReader,
                                      sampleMidiRange,
                                      assignToMidiNoteNumber,   // midi note
                                      attackTime,               // attack time in seconds
                                      releaseTime,              // release time in seconds
                                      maxSampleLength));        // maximum sample length in seconds
}

class MainComponent : public AudioAppComponent, private Timer {
public:
    MainComponent() {
        // set the size of the component after
        // you add any child components.
        setSize(initialWidth, initialHeight);
        
        setAudioChannels(AUDIO_INPUT_CHANNELS, AUDIO_OUTPUT_CHANNELS);

        // polyphony
        for (int count = 0; count < MAX_POLYPHONY; ++count) {
            sampler.addVoice(new SamplerVoice());
        }
        
        // load up a kit TODO working with a kits like abstraction is likely easier
        samplerLoadNewSample(&sampler, "./example.wav", 60);
     
        // start timer as soon as we boot
        startTimerHz(timerIntervalHz);
        
    }
    ~MainComponent() {
        shutdownAudio();
        sampler.clearSounds();
        sampler.clearVoices();
    }
    
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override {
        // dummy midi buffer
        MidiBuffer dummyMidiBuffer;
        sampler.renderNextBlock(*bufferToFill.buffer,
                                dummyMidiBuffer,
                                bufferToFill.startSample,
                                bufferToFill.numSamples);
    }
    
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override {
        sampler.setCurrentPlaybackSampleRate(sampleRate);
    }
    
    void releaseResources() override {}

    void timerCallback() override {
        
        time_t now = time(0);
        std::cout << now << "\n";
    
        int note = composer.tick();
        if (note == 0) {
            return;
        }
        
    }

private:
    int initialWidth{486};
    int initialHeight{864};
    int bpm{96};
    float timerIntervalHz{(bpm / 60.f) * 12};
    Synthesiser sampler;
    Composer composer;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

class dvoinoiApplication  : public JUCEApplication
{
public:
    dvoinoiApplication() {}
    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return true; }
    
    void initialise(const String& commandLine) override
    {
        mainWindow.reset(new MainWindow(getApplicationName()));
    }
    
    void shutdown() override
    {
        // TODO close all connections + ports
        mainWindow = nullptr;
    }
    
    void systemRequestedQuit() override
    {
        quit();
    }
    
    void anotherInstanceStarted(const String& commandLine) override
    {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
    }
    
    class MainWindow    : public DocumentWindow
    {
    public:
        MainWindow(String name) :
        DocumentWindow(name,
                       Desktop::getInstance().getDefaultLookAndFeel().findColour(ResizableWindow::backgroundColourId),
                       DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar(true);
            setContentOwned(new MainComponent(), true);
            setResizable(true, true);
            centreWithSize(getWidth(), getHeight());
            setVisible(false); // we do not need to see a GUI
        }
        
        void closeButtonPressed() override
        {
            JUCEApplication::getInstance()->systemRequestedQuit();
        }
        
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
    };
    
private:
    std::unique_ptr<MainWindow> mainWindow;
};


// main
START_JUCE_APPLICATION(dvoinoiApplication)
