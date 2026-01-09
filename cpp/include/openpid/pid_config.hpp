#pragma once
#include "types.hpp"

namespace openpid {

enum class AntiWindupMode : int {
  NONE = 0,
  CLAMP = 1,
  CONDITIONAL_INTEGRATION = 2,
  BACK_CALCULATION = 3,
};

struct PidConfig {
  Real kp = 0.0;
  Real ki = 0.0;
  Real kd = 0.0;

  Real output_min = 0.0;
  Real output_max = 1.0;

  AntiWindupMode anti_windup = AntiWindupMode::CONDITIONAL_INTEGRATION;
  Real backcalc_gain = 1.0;

  Real derivative_tau = 0.0;
  bool derivative_on_measurement = true;

  Real rate_limit = 0.0;

  bool freeze_integrator_on_large_dt = true;
  Real max_dt_for_integration = 0.5;
};

} 
