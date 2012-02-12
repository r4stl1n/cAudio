# Version 2.2.0 dev

This is the new cAudio 2.2.0 Dev . This release is mainly a bug fix release.

Visit our site at http://caudio.deathtouchstudios.com/ for news and information.

To get help, report bugs, or talk about cAudio visit our forums at: http://www.memoryoffset.com/forum

## Change log

* C# Bindings as well has tutorials and demo application
* CMake Build System
* Remove unneeded mutex/locks from dataFactory->CreateDataSource
* Replace include guards with #pragma once
* Bug fixes,
* cAudio Source are now cleared

# Compiling

## Windows

To compile:

Install CMake if you don't already have it. http://www.cmake.org/cmake/resources/software.html

To generate makefiles use: `cmake . -DCAUDIO_DEPENDENCIES <Path to the Dependencies folder>`. You can also use the graphical CMake utility and set the option in the list before you run generate.

## Linux

This is the build steps for building on a cleanly installed Ubuntu 11.10 system. Other systems should be similar and the build easy to adapt. If you run in to problems post an issue here and if you adapt these steps for another Linux distribution please contribute the information back so it can be included here. Solutions for common problems can be found after the build description.

* First we need some basic build tools. The command `sudo apt-get install build-essential git cmake` will set you up with all you need for this build.

* Then lets get some place for the project to live. `mkdir -p ~/Documents/projects` will set up what I use as a root for all my projects. (You can, of cause, choose any directory you like for this. The GIT command will make a cAudio sub directory for you so you don't have to make that.)
Lets move in to the new directory `cd ~/Documents/projects`.

* Next step is to clone the Github repo for cAudio. Just run `git clone https://github.com/wildicv/cAudio.git` to get your local copy of the source.

* When the clone is done move in to the new directory `cd cAudio/CMake`.

* Install the libraries needed for cAudio to build `sudo apt-get install libopenal-dev libogg-dev`. This will get you the OpenAL and OGG development files (headers and binaries).

* You are now all set to generate the project of your choice. CMake defaults to unix makefiles but you can specify another with the -G switch. Run `cmake ../ -DCAUDIO_DEPENDENCIES_DIR=../Dependencies/` for a 32bit build or `cmake ../ -DCAUDIO_DEPENDENCIES_DIR=../Dependencies64/` for a 64bit build.

* If CMake exits cleanly, run `make -j4` (The -j switch means 'jobs' and tells make how many threads to run. Twice the number of cores you have is recommended.)
The project will now build and you can run an optional `sudo make install` to install the build .so files and headers system wide.

## Common problems and solutions

None reported so far.