/*==============================================================================
//  PluginProcessor.cpp
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

FILTERHEADERS


//==============================================================================
FILTERCLASSNAME::FILTERCLASSNAME()
{
}

FILTERCLASSNAME::~FILTERCLASSNAME()
{
}


void FILTERCLASSNAME::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
	
	/*float* leftData = buffer.getWritePointer(0);  //leftData references left channel now
	float* rightData = buffer.getWritePointer(1); //right data references right channel now
	for (long i = 0; i < buffer.getNumSamples(); i++)
	{
		leftData[i] = 0;
		rightData[i] = 0;
	}*/
}

//==============================================================================
void FILTERCLASSNAME::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
}

void FILTERCLASSNAME::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
}

//==============================================================================
void FILTERCLASSNAME::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback initialization
}

void FILTERCLASSNAME::releaseResources()
{
    // When playback stops, you can use this to free up any spare memory, etc.
}

//==============================================================================
const String FILTERCLASSNAME::getName() const
{
    return JucePlugin_Name;
}

bool FILTERCLASSNAME::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FILTERCLASSNAME::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FILTERCLASSNAME::silenceInProducesSilenceOut() const
{
    return false;
}

double FILTERCLASSNAME::getTailLengthSeconds() const
{
    return 0.0;
}

int FILTERCLASSNAME::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FILTERCLASSNAME::getCurrentProgram()
{
    return 0;
}

void FILTERCLASSNAME::setCurrentProgram (int index)
{
}

const String FILTERCLASSNAME::getProgramName (int index)
{
    return String();
}

void FILTERCLASSNAME::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
bool FILTERCLASSNAME::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* FILTERCLASSNAME::createEditor()
{
    return new EDITORCLASSNAME (*this);
}



//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FILTERCLASSNAME();
}
