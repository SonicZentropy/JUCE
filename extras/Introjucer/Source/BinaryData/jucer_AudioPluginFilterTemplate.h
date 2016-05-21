/*==============================================================================
//  PluginProcessor.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date DATE
//  Copyright (C) 2016 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Base Processor Class
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#ifndef HEADERGUARD
#define HEADERGUARD

APPHEADERS

//==============================================================================
class FILTERCLASSNAME  : public AudioProcessor
{
public:
    //==============================================================================
    FILTERCLASSNAME();
    ~FILTERCLASSNAME();

	void processBlock (AudioSampleBuffer&, MidiBuffer&) override;
	
	//==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
	
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    float getSampleRate() const { return sampleRate; }
	void setSampleRate(float inValue) { sampleRate = inValue; }
    
private:
    ScopedPointer<ZenDebugEditor> debugWindow;
    float sampleRate = 44100.0f;	
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FILTERCLASSNAME)
};


#endif  // HEADERGUARD
