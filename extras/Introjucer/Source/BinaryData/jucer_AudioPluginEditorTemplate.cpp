/*==============================================================================
//  PluginProcessorEditor.cpp
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date DATE
//  Copyright (C) 2016 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Base Editor Class
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

EDITORCPPHEADERS


//==============================================================================
EDITORCLASSNAME::EDITORCLASSNAME (FILTERCLASSNAME& p)
    : AudioProcessorEditor (&p), processor (p)
{

    setSize (400, 300);
    ZEN_COMPONENT_DEBUG_ATTACH(this);
}

EDITORCLASSNAME::~EDITORCLASSNAME()
{
    
    ZenDebugEditor::removeComponentDebugger();
}

//==============================================================================
void EDITORCLASSNAME::paint (Graphics& g)
{
    g.fillAll (Colours::black);
}

void EDITORCLASSNAME::resized()
{

}
