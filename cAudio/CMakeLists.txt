PROJECT(cAudio)
	
if(CAUDIO_STATIC)
	ADD_DEFINITIONS(-DCAUDIO_STATIC_LIB)
endif()

# generate cAudioBuildSettings.h 
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../CMake/Templates/cAudioBuildSettings.h.in ${CMAKE_BINARY_DIR}/include/cAudioBuildSettings.h @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/include/cAudioBuildSettings.h DESTINATION include/cAudio)
INSTALL(DIRECTORY include/ DESTINATION include/cAudio FILES_MATCHING PATTERN "*.h")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})
set(CAUDIO_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_BINARY_DIR}/include PARENT_SCOPE)

include(cAudio_src_files.cmake)		
add_library(cAudio ${CAUDIO_LIB_TYPE} ${file_root})

if(CAUDIO_ENABLE_OGG)
	include_directories (include Headers ${CMAKE_BINARY_DIR}/include ${OPENAL_INCLUDE_DIRS} ${OGG_INCLUDE_DIR} ${VORBIS_INCLUDE_DIR})
	target_link_libraries(cAudio ${VORBIS_LIBRARIES} ${OGG_LIBRARIES} ${OPENAL_LIBRARIES})
	if(NOT CAUDIO_SYSTEM_OGG)
		add_dependencies(cAudio Vorbis Ogg)
	endif()
else()
	include_directories (include Headers ${CMAKE_BINARY_DIR}/include ${OPENAL_INCLUDE_DIRS})
	target_link_libraries(cAudio ${OPENAL_LIBRARIES})
endif()

set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY VERSION "2.3.0")
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY SOVERSION 2 )

if (APPLE AND CAUDIO_IOS_BUILD)
	set_target_properties(cAudio PROPERTIES XCODE_ATTRIBUTE_GCC_THUMB_SUPPORT "NO")
	set_target_properties(cAudio PROPERTIES XCODE_ATTRIBUTE_GCC_UNROLL_LOOPS "YES")
	set_target_properties(cAudio PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer")
endif()

install_all_targets(cAudio)
