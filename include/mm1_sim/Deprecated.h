//
// Unfortunately in C++ 11 no deprecated tags supported
//

#ifndef DEPRECATED_H
#define DEPRECATED_H

#if defined(__GNUC__) || defined(__clang__)
  #define DEPRECATED(msg) __attribute__((deprecated(msg)))
#elif defined(_MSC_VER)
  #define DEPRECATED(msg) __declspec(deprecated(msg))
#else
  #pragma message("WARNING: You need to implement DEPRECATED for this compiler")
  #define DEPRECATED(msg)
#endif

#endif //DEPRECATED_H
