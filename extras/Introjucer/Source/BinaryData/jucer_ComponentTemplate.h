/*==============================================================================
//  FILENAME
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date DATE
//  Copyright (C) 2016 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Generic GUI Component Header
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#ifndef %%headerGuard%%
#define %%headerGuard%%

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
//[/Headers]

%%includeFilesH%%

//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
%%classDeclaration%%
{
public:
    //==============================================================================
    %%className%% (%%constructorParams%%);
    ~%%className%%();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    %%publicMemberDeclarations%%

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    %%privateMemberDeclarations%%

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (%%className%%)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // %%headerGuard%%
