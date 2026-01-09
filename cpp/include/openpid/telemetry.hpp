#pragma once
#include "types.hpp"

namespace openpid {

struct Telemetry {
  Real error = 0.0;
  Real p = 0.0;
  Real i = 0.0;
  Real d = 0.0;

  Real u_unsat = 0.0;
  Real u = 0.0;

  bool saturated = false;

  Real integrator = 0.0;
  Real d_filtered = 0.0;
};

} 
