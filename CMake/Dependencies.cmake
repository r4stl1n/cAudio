#######################################################################
# Find all necessary and optional CAUDIO dependencies
#######################################################################

# CAUDIO_DEPENDENCIES_DIR can be used to specify a single base
# folder where the required dependencies may be found.
set(CAUDIO_DEPENDENCIES_DIR "" CACHE PATH "Path to prebuilt CAUDIO dependencies")

include(FindPkgMacros)
getenv_path(CAUDIO_DEPENDENCIES_DIR)
message(STATUS "Search path: ${CAUDIO_DEPENDENCIES_DIR}")

# give guesses as hints to the find_package calls
set(CMAKE_PREFIX_PATH ${CAUDIO_DEPENDENCIES_DIR} ${CMAKE_PREFIX_PATH})
set(CMAKE_FRAMEWORK_PATH ${CAUDIO_DEPENDENCIES_DIR} ${CMAKE_FRAMEWORK_PATH})

#######################################################################
# Core dependencies
#######################################################################

# Find OpenAL
find_package(OpenAL)
macro_log_feature(OPENAL_FOUND "OpenAL" "Support for Audio" "http://connect.creativelabs.com/openal/default.aspx" TRUE "" "")

# Display results, terminate if anything required is missing
MACRO_DISPLAY_FEATURE_LOG()

# Add library and include paths from the dependencies
include_directories(
  ${OPENAL_INCLUDE_DIR}
)

# On Windows it's possible to get an include path that directly contains the headers.
# This breaks the build so here's a fix that requires no changes to the source files
if(WIN32)
  include_directories(
    "${OPENAL_INCLUDE_DIR}/../"
  )
endif()
