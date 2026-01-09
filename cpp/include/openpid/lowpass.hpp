#pragma once
#include <algorithm>
#include "types.hpp"

namespace openpid {

class FirstOrderLowpass {
public:
  explicit FirstOrderLowpass(Real tau = 0.0) : tau_(tau) {}

  void set_tau(Real tau) { tau_ = std::max<Real>(0.0, tau); }
  Real tau() const { return tau_; }

  Real step(Real x, Real dt, bool& has_state, Real& state) const {
    if (tau_ <= 0.0) {
      state = x;
      has_state = true;
      return x;
    }
    const Real alpha = dt / (tau_ + dt);
    if (!has_state) {
      state = x;
      has_state = true;
      return x;
    }
    state = state + alpha * (x - state);
    return state;
  }

private:
  Real tau_;
};

} 
