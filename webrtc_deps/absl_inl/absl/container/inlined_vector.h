#ifndef ABSL_CONTAINER_INLINED_VECTOR_H_
#define ABSL_CONTAINER_INLINED_VECTOR_H_

#include <vector>

namespace absl {

 template <typename T, size_t N, typename A = std::allocator<T>>
 using InlinedVector = std::vector<T, A>;

}
#endif  // ABSL_CONTAINER_INLINED_VECTOR_H_
