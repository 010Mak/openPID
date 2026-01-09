#pragma once
#include "types.hpp"

namespace openpid {

struct PidState {
  Real integrator = 0.0;

  bool has_prev = false;
  Real prev_error = 0.0;
  Real prev_measurement = 0.0;

  bool has_d_filt = false;
  Real d_filtered = 0.0;

  bool has_prev_u = false;
  Real prev_u = 0.0;
};

} 
