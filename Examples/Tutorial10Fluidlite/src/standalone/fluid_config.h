/* src/config.h.in.  Generated from configure.ac by autoheader.  */


/* Define to activate debugging message */
#undef DEBUG

/* Version number of package */
#define VERSION "1.0.9"

/* Define to 1 if your processor stores words with the most significant byte
   first (like Motorola and SPARC, unlike Intel and VAX). */
#undef WORDS_BIGENDIAN

#define SF3_DISABLED 0
#define SF3_XIPH_VORBIS 1
#define SF3_STB_VORBIS 2

#ifndef SF3_SUPPORT
#define SF3_SUPPORT SF3_DISABLED
#endif

#ifndef WITH_FLOAT
#define WITH_FLOAT 1
#endif

#define HAVE_STRING_H 1
#define HAVE_STDLIB_H 1
#define HAVE_STDIO_H 1
#define HAVE_MATH_H 1
#define HAVE_STDARG_H 1
#define HAVE_FCNTL_H 1
#define HAVE_LIMITS_H 1


//#pragma warning(disable : 4244)
//#pragma warning(disable : 4101)
//#pragma warning(disable : 4305)
//#pragma warning(disable : 4996)
