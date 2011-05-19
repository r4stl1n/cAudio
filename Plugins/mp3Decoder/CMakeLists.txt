#-------------------------------------------------------------------
# This file is part of the CMake build system for cAudio
#-------------------------------------------------------------------

############################################################
# mp3Decoder library
############################################################

PROJECT(mp3Decoder)

if(WIN32)
	if(${CAUDIO_STATIC})
		ADD_DEFINITIONS(-DCAUDIO_STATIC_LIB)
	endif()
endif() 

include_directories (include ${CAUDIO_INCLUDE_DIR} )
include(mp3Decoder_src_files.cmake)
add_library(mp3Decoder ${CAUDIO_LIB_TYPE} ${file_root})

target_link_libraries(mp3Decoder cAudio)
add_dependencies(mp3Decoder cAudio)

if(${CAUDIO_STATIC})
	ADD_DEFINITIONS(-DCAUDIO_STATIC_LIB)
endif()

install_all_targets(mp3Decoder)