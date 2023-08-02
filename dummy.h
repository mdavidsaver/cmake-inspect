#ifndef DUMMY_H
#define DUMMY_H

#if defined(_WIN32) || defined(__CYGWIN__)

#  if defined(DUMMY_API_BUILDING) && defined(EPICS_BUILD_DLL)
/* building library as dll */
#    define DUMMY_API __declspec(dllexport)
#  elif !defined(DUMMY_API_BUILDING) && defined(EPICS_CALL_DLL)
/* calling library in dll form */
#    define DUMMY_API __declspec(dllimport)
#  endif

#elif __GNUC__ >= 4
#  define DUMMY_API __attribute__ ((visibility("default")))
#endif

#ifndef DUMMY_API
#  define DUMMY_API
#endif

int DUMMY_API some_stupid_code(void);

#endif /* DUMMY_H */
