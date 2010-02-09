#ifndef CAUDIOPLATFORM_H_INCLUDED
#define CAUDIOPLATFORM_H_INCLUDED

#ifndef CAUDIO_PLATFORM_WIN
#	if defined( WIN32 ) || defined( _WINDOWS ) || defined(_WIN32)
#		define CAUDIO_PLATFORM_WIN
#	endif
#endif

#ifndef CAUDIO_PLATFORM_MAC
#   if defined( __APPLE__ ) || defined( __APPLE_CC__ )
#      define CAUDIO_PLATFORM_MAC
#   endif
#endif

#ifndef CAUDIO_PLATFORM_LINUX
#	if defined(_UNIX) || defined(__linux)
#		define CAUDIO_PLATFORM_LINUX
#	endif
#endif

#endif //! CAUDIOPLATFORM_H_INCLUDED