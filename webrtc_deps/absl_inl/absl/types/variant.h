#ifndef ABSL_VARIANT_H
#define ABSL_VARIANT_H

#include <variant>

namespace  absl{

    using  std::variant;

    using std::bad_variant_access;
    using std::get;
    using std::get_if;
    using std::holds_alternative;
    using std::monostate;
    using std::variant;
    using std::variant_alternative;
    using std::variant_alternative_t;
    using std::variant_npos;
    using std::variant_size;
    using std::variant_size_v;
    using std::visit;
}
#endif /*ABSL_VARIANT_H*/
