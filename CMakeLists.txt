#/**********************************************************\ 
#Original Author: Murat Sari (wolfmanfx)
#
#Created:    Feb 20, 2011
#License:    ZLib
#            
#\**********************************************************/
cmake_minimum_required(VERSION 2.8)

project(cAudio)

# Include necessary submodules
set(CMAKE_MODULE_PATH 
  "${CMAKE_SOURCE_DIR}/CMake"
  "${CMAKE_SOURCE_DIR}/CMake/Utils" 
  "${CMAKE_SOURCE_DIR}/CMake/Packages"
)

include(CMakeDependentOption)
include(PreprocessorUtils)
include(MacroLogFeature)

#####################################################################
# Set up the basic build environment
#####################################################################

if (CMAKE_BUILD_TYPE STREQUAL "")
	# CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
	# differentiation between debug and release builds.
	set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()

if (WIN32)
	# Create debug libraries with _d postfix
	set(CMAKE_DEBUG_POSTFIX "_d")
endif()


set(CAUDIO_RELEASE_PATH "/Release")
set(CAUDIO_RELWDBG_PATH "/RelWithDebInfo")
set(CAUDIO_MINSIZE_PATH "/MinSizeRel")
set(CAUDIO_DEBUG_PATH "/Debug")
set(CAUDIO_LIB_DIRECTORY "/lib")  
 
######################################################################
# Provide user options to customise the build process
######################################################################

# Customise what to build
option(CAUDIO_STATIC "Static build" FALSE)
option(CAUDIO_BUILD_SAMPLES "Build Samples" TRUE)
option(CAUDIO_BUILD_CSHARP_WRAPPER "Build CSharpWrapper - this wrapper is used with the provided *.cs files" FALSE)

#Custom settings 
option(CAUDIO_BUILD_EAX_PLUGIN "Build EAXLegacyPreset Plugin" FALSE)
option(CAUDIO_BUILD_MP3DECODER_PLUGIN "Build mp3Decoder Plugin" FALSE)
option(CAUDIO_ENABLE_OGG "Enable OGG decoder" TRUE)
option(CAUDIO_ENABLE_WAV "Enable RIFF/Wav decoder" TRUE)
option(CAUDIO_ENABLE_DEFAULT_FILESYSTEM "Enable default filesystem data source" TRUE)
option(CAUDIO_ENABLE_DEFAULT_FILE_LOGGER "Enable default file logger (html)" TRUE)
option(CAUDIO_ENABLE_DEFAULT_CONSOLE_LOGGER "Enable default console logger" TRUE)
option(CAUDIO_ENABLE_THREAD_SAFETY "Enable thread safety" TRUE)
option(CAUDIO_ENABLE_STD_MEMORY_ALLOCATOR "Enable Std Memory allocations (memalloc and free)" TRUE)
option(CAUDIO_ENABLE_REROUTE_STL_ALLOCATIONS "Reroute STL memory allocations" TRUE)
option(CAUDIO_ENABLE_MEMORYTRACKER "Enable memory leak tracker" FALSE)
option(CAUDIO_ENABLE_MEMORY_LOG_ALL_ALLOCATIONS "Enable log all memory allocations" FALSE)
option(CAUDIO_ENABLE_MEMORY_STATISTICS "Enable memory statistics generator" FALSE)
if(APPLE)
	option(CAUDIO_IOS_BUILD "Build for ios" FALSE)
endif()

include(ConfigureBuild)

if (CAUDIO_IOS_BUILD)
  # Set build variables
  set(CMAKE_OSX_SYSROOT iphoneos)
  set(CMAKE_OSX_DEPLOYMENT_TARGET "")
  set(CMAKE_EXE_LINKER_FLAGS "-framework Foundation -framework CoreGraphics -framework QuartzCore -framework UIKit -framework OpenGLES")
  set(XCODE_ATTRIBUTE_SDKROOT iphoneos)
  set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.YOUR_COMPANY.\${PRODUCT_NAME:rfc1034identifier}")

  set(CMAKE_OSX_ARCHITECTURES $(ARCHS_STANDARD_32_BIT))

  add_definitions(-fno-regmove)
  add_definitions(-falign-loops=16)
  remove_definitions(-msse)

elseif (APPLE AND NOT CAUDIO_IOS_BUILD)

  # Set 10.5 as the base SDK by default
  set(XCODE_ATTRIBUTE_SDKROOT macosx)
  set(CMAKE_OSX_SYSROOT macosx)
  set(CMAKE_OSX_DEPLOYMENT_TARGET 10.5)

  if (NOT CMAKE_OSX_ARCHITECTURES)
    set(CMAKE_OSX_ARCHITECTURES $(ARCHS_STANDARD_32_64_BIT))
  endif()
endif()

include(Dependencies)
include(InstallDependencies)

if(CAUDIO_ENABLE_OGG)
	add_subdirectory(DependenciesSource/libogg-1.2.2)
	add_subdirectory(DependenciesSource/libvorbis-1.3.2)
endif()

add_subdirectory(cAudio)

if(CAUDIO_BUILD_EAX_PLUGIN)
	add_subdirectory(Plugins/EAXLegacyPreset)
endif()

if(CAUDIO_BUILD_MP3DECODER_PLUGIN)
	add_subdirectory(Plugins/mp3Decoder)
endif()

if(CAUDIO_BUILD_CSHARP_WRAPPER)
	add_subdirectory(cAudioCSharpWrapper)
endif()

if(CAUDIO_BUILD_SAMPLES)
	ADD_DEFINITIONS(-DCAUDIO_MEDIA_ROOT="${CMAKE_SOURCE_DIR}/Samples/Media/")
	add_subdirectory(Samples/Tutorial1_2DSound)
	add_subdirectory(Samples/Tutorial2_3DSound)	
	add_subdirectory(Samples/Tutorial3_MemoryPlayback)	
	add_subdirectory(Samples/Tutorial4_AudioCapture)	
	add_subdirectory(Samples/Tutorial5_AudioEffects)	
	add_subdirectory(Samples/Tutorial6_CustomEventHandler)	
	add_subdirectory(Samples/Tutorial7_CustomLogReceiver)	
endif()