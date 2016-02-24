/*==============================================================================
//  FILENAME
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date DATE
//  Copyright (C) 2016 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Generic Content Component Class
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

INCLUDE_CORRESPONDING_HEADER


//==============================================================================
CONTENTCOMPCLASS::CONTENTCOMPCLASS()
{
    setSize (600, 400);
}

CONTENTCOMPCLASS::~CONTENTCOMPCLASS()
{
}

void CONTENTCOMPCLASS::paint (Graphics& g)
{
    g.fillAll (Colour (0xff001F36));
}

void CONTENTCOMPCLASS::resized()
{
    // This is called when the CONTENTCOMPCLASS is resized.
    // If you add any child components, this is where you should
    // update their positions.
}
