#ifndef ABSL_TYPETRAITS_H
#define ABSL_TYPETRAITS_H

#include <absl_include.h>

// Defines the default alignment. `__STDCPP_DEFAULT_NEW_ALIGNMENT__` is a C++17
// feature.
#if defined(__STDCPP_DEFAULT_NEW_ALIGNMENT__)
#define ABSL_INTERNAL_DEFAULT_NEW_ALIGNMENT __STDCPP_DEFAULT_NEW_ALIGNMENT__
#else  // defined(__STDCPP_DEFAULT_NEW_ALIGNMENT__)
#define ABSL_INTERNAL_DEFAULT_NEW_ALIGNMENT alignof(std::max_align_t)
#endif  // defined(__STDCPP_DEFAULT_NEW_ALIGNMENT__)

namespace absl{

template<typename T>
using remove_reference_t = typename std::remove_reference<T>::type;

}

#endif /*ABSL_TYPETRAITS_H*/
