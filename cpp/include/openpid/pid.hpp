#pragma once
#include "types.hpp"
#include "pid_config.hpp"
#include "pid_state.hpp"
#include "telemetry.hpp"
#include "lowpass.hpp"
#include "ratelimit.hpp"

namespace openpid {

class PidController {
public:
  explicit PidController(const PidConfig& cfg);

  void reset(Real integrator = 0.0);

  
  Telemetry step(Real setpoint, Real measurement, Real dt);

  
  Real step_u(Real setpoint, Real measurement, Real dt);

private:
  PidConfig cfg_;
  PidState st_;
  FirstOrderLowpass d_lp_;
  SlewRateLimiter rate_;

  bool should_integrate(Real dt) const;

  
  Real step_impl(Real setpoint, Real measurement, Real dt, Telemetry* out);
};

} 
