/*==============================================================================
//  FILENAME
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date DATE
//  Copyright (C) 2016 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Generic GUI Component Class
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

//[Headers] You can add your own extra header files here...
//[/Headers]

%%includeFilesCPP%%

//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
%%className%%::%%className%% (%%constructorParams%%)
%%initialisers%%{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    %%constructor%%

    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

%%className%%::~%%className%%()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    %%destructor%%

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
%%methodDefinitions%%

//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

%%metadata%%
END_JUCER_METADATA
*/
#endif

%%staticMemberDefinitions%%
//[EndFile] You can add extra defines here...
//[/EndFile]
