#pragma once
#include <algorithm>
#include "types.hpp"

namespace openpid {

inline bool is_saturated(Real u_unsat, Real u_sat) {
  return u_unsat != u_sat;
}

inline Real clamp_output(Real u, Real umin, Real umax) {
  return std::clamp(u, umin, umax);
}

} 
