#-------------------------------------------------------------------
# This file is part of the CMake build system for cAudio
#-------------------------------------------------------------------

############################################################
# cAp_mp3Decoder library
############################################################

PROJECT(cAp_mp3Decoder)

if(WIN32)
	if(${CAUDIO_STATIC})
		ADD_DEFINITIONS(-DCAUDIO_STATIC_LIB)
	endif()
endif() 

include_directories (include ${CAUDIO_INCLUDE_DIR} )
include(mp3Decoder_src_files.cmake)
add_library(cAp_mp3Decoder ${CAUDIO_LIB_TYPE} ${file_root})

target_link_libraries(cAp_mp3Decoder cAudio)
add_dependencies(cAp_mp3Decoder cAudio)

if(${CAUDIO_STATIC})
	ADD_DEFINITIONS(-DCAUDIO_STATIC_LIB)
endif()

set_property(TARGET cAp_mp3Decoder PROPERTY VERSION "2.3.0")
set_property(TARGET cAp_mp3Decoder PROPERTY SOVERSION 2 )

install_all_targets(cAp_mp3Decoder)
