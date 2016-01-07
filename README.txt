//////////////////////////////////////////////////////////////////////////////

Project:    AlarmME
Author:     Ryan Luu  (ryanluu@gmail.com)
Version:    1.0.0
Date:       May 27, 2005.

Project website: https://rluu.github.io/alarmme/

//////////////////////////////////////////////////////////////////////////////

AlarmME v 1.0.0
Copyright (C) 2005  Ryan Luu

//////////////////////////////////////////////////////////////////////////////

Description:

AlarmME ('Alarm Made Easy') is an alarm clock program which will work
as an alarm clock, playing a media file as the alarm.  This program
supports Windows, Mac, and Linux platforms and supports many different
media players.  It features re-alarming intervals and auto-saving of
the user settings.

Dependencies:

This program uses the Trolltech Qt 3.2 development framework, which is
included as an installable package (QtLibrary-setup-3.2.1.exe).

//////////////////////////////////////////////////////////////////////////////

Building from source:

Dependencies:
  - Qt 3.2 library (or higher).

Run the following commands to build this software and run it: 

# Change to the src directory.
cd src

# Run qmake to generate the project Makefile.  
qmake

# Run make to compile.
make

# Run the application.
./AlarmMe

//////////////////////////////////////////////////////////////////////////////
