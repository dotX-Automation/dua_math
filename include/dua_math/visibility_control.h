#ifndef DUA_MATH__VISIBILITY_CONTROL_H_
#define DUA_MATH__VISIBILITY_CONTROL_H_

// This logic was borrowed (then namespaced) from the examples on the gcc wiki:
//     https://gcc.gnu.org/wiki/Visibility

#if defined _WIN32 || defined __CYGWIN__
  #ifdef __GNUC__
    #define DUA_MATH_EXPORT __attribute__ ((dllexport))
    #define DUA_MATH_IMPORT __attribute__ ((dllimport))
  #else
    #define DUA_MATH_EXPORT __declspec(dllexport)
    #define DUA_MATH_IMPORT __declspec(dllimport)
  #endif
  #ifdef DUA_MATH_BUILDING_LIBRARY
    #define DUA_MATH_PUBLIC DUA_MATH_EXPORT
  #else
    #define DUA_MATH_PUBLIC DUA_MATH_IMPORT
  #endif
  #define DUA_MATH_PUBLIC_TYPE DUA_MATH_PUBLIC
  #define DUA_MATH_LOCAL
#else
  #define DUA_MATH_EXPORT __attribute__ ((visibility("default")))
  #define DUA_MATH_IMPORT
  #if __GNUC__ >= 4
    #define DUA_MATH_PUBLIC __attribute__ ((visibility("default")))
    #define DUA_MATH_LOCAL  __attribute__ ((visibility("hidden")))
  #else
    #define DUA_MATH_PUBLIC
    #define DUA_MATH_LOCAL
  #endif
  #define DUA_MATH_PUBLIC_TYPE
#endif

#endif  // DUA_MATH__VISIBILITY_CONTROL_H_
