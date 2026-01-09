#pragma once
#include <algorithm>
#include "types.hpp"

namespace openpid {

class SlewRateLimiter {
public:
  explicit SlewRateLimiter(Real max_rate = 0.0) : max_rate_(max_rate) {}

  void set_max_rate(Real r) { max_rate_ = std::max<Real>(0.0, r); }
  Real max_rate() const { return max_rate_; }

  Real step(Real u, Real dt, bool& has_prev, Real& prev_u) const {
    if (max_rate_ <= 0.0 || dt <= 0.0) {
      prev_u = u;
      has_prev = true;
      return u;
    }
    if (!has_prev) {
      prev_u = u;
      has_prev = true;
      return u;
    }
    const Real du_max = max_rate_ * dt;
    const Real du = u - prev_u;
    const Real du_clamped = std::clamp(du, -du_max, du_max);
    prev_u = prev_u + du_clamped;
    return prev_u;
  }

private:
  Real max_rate_;
};

} 
