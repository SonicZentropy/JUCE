/*==============================================================================
//  PluginProcessorEditor.h
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

#ifndef HEADERGUARD
#define HEADERGUARD

EDITORHEADERS


//==============================================================================
/**
*/
class EDITORCLASSNAME  : public AudioProcessorEditor
{
public:
    explicit EDITORCLASSNAME (FILTERCLASSNAME&);
    ~EDITORCLASSNAME();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    FILTERCLASSNAME& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EDITORCLASSNAME)
};


#endif  // HEADERGUARD
