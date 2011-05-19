PROJECT(cAudio)
	
if(CAUDIO_STATIC)
	ADD_DEFINITIONS(-DCAUDIO_STATIC_LIB)
endif()

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})
set(CAUDIO_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)

include_directories (include ${OPENAL_INCLUDE_DIRS} ${OGG_INCLUDE_DIR} ${VORBIS_INCLUDE_DIR})
include(cAudio_src_files.cmake)

add_library(cAudio ${CAUDIO_LIB_TYPE} ${file_root})
target_link_libraries(cAudio Vorbis Ogg ${OPENAL_LIBRARIES})
add_dependencies(cAudio Vorbis Ogg)

if (APPLE AND CAUDIO_IOS_BUILD)
	set_target_properties(cAudio PROPERTIES XCODE_ATTRIBUTE_GCC_THUMB_SUPPORT "NO")
	set_target_properties(cAudio PROPERTIES XCODE_ATTRIBUTE_GCC_UNROLL_LOOPS "YES")
	set_target_properties(cAudio PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer")
endif()

install_all_targets(cAudio)
