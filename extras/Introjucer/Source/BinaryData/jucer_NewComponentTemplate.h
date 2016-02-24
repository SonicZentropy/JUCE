/*==============================================================================
//  FILENAME
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date DATE
//  Copyright (C) 2016 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Generic Component Header
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#ifndef HEADERGUARD
#define HEADERGUARD

INCLUDE_JUCE

//==============================================================================
/*
*/
class COMPONENTCLASS    : public Component
{
public:
    COMPONENTCLASS();
    ~COMPONENTCLASS();

    void paint (Graphics&);
    void resized();

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (COMPONENTCLASS)
};


#endif  // HEADERGUARD
