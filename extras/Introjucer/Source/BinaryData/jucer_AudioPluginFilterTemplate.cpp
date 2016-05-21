/*==============================================================================
//  FILTERCLASSNAME.cpp
//  Part of the SonicZentropy JUCE Collection
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
#ifdef JUCE_MSVC
    //Visual Studio mem leak diagnostics settings 
	_CrtSetDbgFlag(0);	//Turn off VS memory dump output
	//_crtBreakAlloc = 307;	//Break on this memory allocation number (When Debug)
#endif
    
#ifdef JUCE_DEBUG
	//Create Zen_Utils Debug window instance
    debugWindow = ZenDebugEditor::getInstance();
	debugWindow->setSize(400, 400);
	debugWindow->setTopLeftPosition(1900 - debugWindow->getWidth(), 1040 - debugWindow->getHeight());
#endif
}

FILTERCLASSNAME::~FILTERCLASSNAME()
{
    debugWindow = nullptr;
}


void FILTERCLASSNAME::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    setCurrentSampleRate(getSampleRate());
    
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears excess output
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's processing
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer (channel);
    }
	
	/* Operate per-sample on each channel instead of per-channel on each sample
	float* leftData = buffer.getWritePointer(0);  //leftData references left channel now
	float* rightData = buffer.getWritePointer(1); //right data references right channel now
	for (long i = 0; i < buffer.getNumSamples(); i++)
	{
		leftData[i] = 0;
		rightData[i] = 0;
		//ZEN_LABEL_TRACE("Left", S(leftData[i]));
	}
	//ZEN_DEBUG_BUFFER("Left Buffer Pre", leftData, buffer.getNumSamples(), -1, 1);
	*/
}

//==============================================================================
void FILTERCLASSNAME::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    /*
	XmlElement rootXML("Root");

	for (auto param : getParameters())
	{
		ZenParameter* zenParam = dynamic_cast<ZenParameter*>(param);
		zenParam->writeToXML(rootXML);
	}
	//DBG(rootXML.createDocument("", false, false, "UTF-8", 120));
	copyXmlToBinary(rootXML, destData);
	*/
}

void FILTERCLASSNAME::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block.
    
    /*
    ScopedPointer<XmlElement> theXML = this->getXmlFromBinary(data, sizeInBytes);
	//DBG(theXML->createDocument("", false, false, "UTF-8", 120));

	if (theXML != nullptr)
	{
		for (auto param : getParameters())
		{
			ZenParameter* zenParam = dynamic_cast<ZenParameter*>(param);
			if(zenParam != nullptr) zenParam->setFromXML(*theXML);
		}
	}
	*/
}

//==============================================================================
void FILTERCLASSNAME::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback initialization
    /*
    // Iterates over parameters and resets Smooth for the ones who need it
	for (auto param : getParameters())
	{
		ZenParameter* zenParam = dynamic_cast<ZenParameter*>(param);
		if (zenParam != nullptr)
		{
			if (zenParam->checkShouldBeSmoothed())
			{
				zenParam->resetSmoothedValue(inSampleRate);
			}
		}
	}
	*/
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

double FILTERCLASSNAME::getTailLengthSeconds() const
{
    return 0.0;
}

int FILTERCLASSNAME::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs.
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
