# Version 2.2.0 dev

This is the new cAudio 2.2.0 Dev . This release is mainly a bug fix release.

Visit our site at http://caudio.deathtouchstudios.com/ for news and information.

To get help, report bugs, or talk about cAudio visit our forums at: http://www.memoryoffset.com/forum

## Changelog

* C# Bindings as well has tutorials and demo application
* Cmake Build System
* Remove unneeded mutex/locks from dataFactory->CreateDataSource
* Replace include guards with #pragma once
* BugFixes,
* cAudio Source are now cleared

# Compiling

## Windows

To compile:

Install CMake if you don't allready have it. http://www.cmake.org/cmake/resources/software.html

To generate makefiles use: `cmake . -DCAUDIO_DEPENDENCIES <Path to the Dependencies folder>`. You can also use the graphical cmake utility

## Linux
