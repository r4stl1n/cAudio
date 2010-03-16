This is the new cAudio 2.1.0 Release. This release is mainly a bug fix release.

Visit our site at http://caudio.deathtouchstudios.com/ for news and information.

To get help, report bugs, or talk about cAudio visit our forums at: http://www.deathtouchstudios.com/phpBB3/index.php

New:  
	New Memory System (allows for the user to override all memory allocations by cAudio, providing a custom allocation scheme if necessary)
	New Memory Tracker (for finding memory leaks and tracking memory usage statistics)
	Official Mac OS X and IPhone support

Updated:
    Removed old memory system due to numerous bugs.
	Added a ogg vorbis close callback function to prevent a crash on Mac/IPhone
	Reorganized cAudioDefines to order by platform.
	Added defines to disable portions or all of the plugin system.
	Added defines for finer control of what is compiled into the library.
	Removed the ogg/vorbis encoding lib that was mistakenly linked in, which doubled the binary's size.
	
Note for linux users:
By default, EFX is disabled on linux with an ifdef in cAudioDefines. We are doing this because of licensing issues with EFX on linux.  We cannot provide in our packages key EFX headers, so if you want EFX support on linux, you must provide them yourself.

Known Bugs:
Looping with small sounds (less than 192 KB with the default buffer settings) does not work properly if you try to change the loop setting while the sound is playing.

//////////////////
// INSTALLATION //
//////////////////

To compile:

	Windows: Open up the cAudio Microsoft Visual C++ 2008 solution (or codeblocks project file for MinGW) and select Build Solution after you set Debug or Release.

	Linux/Unix: Cd to the Source directory and run make.

	Mac: Open up the xcode project file and compile away.

Libraries will end up in:

    Windows: lib/win32-visual (MSVC) or lib/win32-gcc (Codeblocks).

    Linux: lib/linux-x86.

    Mac: bin/osx (bin/osx-iphone for IPhone)

To compile examples:

	Windows: Automatically built by the MSVC Solution.  For codeblocks, open up the projects in the Examples directory select perferred compiler and build configuration.
	
	Linux/Unix: Cd to the Examples directory and run ./buildAll.sh

	Mac: Open up the respective XCode project file and compile away.
	
Compiled examples will be in:

    Windows: bin/win32-visual (MSVC) or bin/win32-gcc (Codeblocks).

    Linux: bin/linux-x86.

    Mac: bin/osx (bin/osx-iphone for IPhone)
