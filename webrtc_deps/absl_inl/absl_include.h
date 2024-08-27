#ifndef ABSL_INCLUDE_H
#define ABSL_INCLUDE_H

#include <type_traits>
#include<memory>
#include <string.h>

namespace absl{

template <typename T>
using decay_t = typename std::decay<T>::type;

template <bool B, typename T = void>
using enable_if_t = typename std::enable_if<B, T>::type;

template <typename T>
using underlying_type_t = typename std::underlying_type<T>::type;

// -----------------------------------------------------------------------------
// Function Template: WrapUnique()
// -----------------------------------------------------------------------------
//
// Adopts ownership from a raw pointer and transfers it to the returned
// `std::unique_ptr`, whose type is deduced. Because of this deduction, *do not*
// specify the template type `T` when calling `WrapUnique`.
//
// Example:
//   X* NewX(int, int);
//   auto x = WrapUnique(NewX(1, 2));  // 'x' is std::unique_ptr<X>.
//
// Do not call WrapUnique with an explicit type, as in
// `WrapUnique<X>(NewX(1, 2))`.  The purpose of WrapUnique is to automatically
// deduce the pointer type. If you wish to make the type explicit, just use
// `std::unique_ptr` directly.
//
//   auto x = std::unique_ptr<X>(NewX(1, 2));
//                  - or -
//   std::unique_ptr<X> x(NewX(1, 2));
//
// While `absl::WrapUnique` is useful for capturing the output of a raw
// pointer factory, prefer 'absl::make_unique<T>(args...)' over
// 'absl::WrapUnique(new T(args...))'.
//
//   auto x = WrapUnique(new X(1, 2));  // works, but nonideal.
//   auto x = make_unique<X>(1, 2);     // safer, standard, avoids raw 'new'.
//
// Note that `absl::WrapUnique(p)` is valid only if `delete p` is a valid
// expression. In particular, `absl::WrapUnique()` cannot wrap pointers to
// arrays, functions or void, and it must not be used to capture pointers
// obtained from array-new expressions (even though that would compile!).
template <typename T>
std::unique_ptr<T> WrapUnique(T* ptr) {
  static_assert(!std::is_array<T>::value, "array types are unsupported");
  static_assert(std::is_object<T>::value, "non-object types are unsupported");
  return std::unique_ptr<T>(ptr);
}
}

#endif/*ABSL_INCLUDE_H*/
