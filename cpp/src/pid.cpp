#include "openpid/pid.hpp"
#include "openpid/antiwindup.hpp"

#include <algorithm>
#include <stdexcept>

namespace openpid {

PidController::PidController(const PidConfig& cfg)
  : cfg_(cfg), d_lp_(cfg.derivative_tau), rate_(cfg.rate_limit) {

  if (!(cfg_.output_min < cfg_.output_max)) {
    throw std::invalid_argument("output_min must be < output_max");
  }
  if (cfg_.derivative_tau < 0.0) {
    throw std::invalid_argument("derivative_tau must be >= 0");
  }
  if (cfg_.rate_limit < 0.0) {
    throw std::invalid_argument("rate_limit must be >= 0");
  }
  if (cfg_.max_dt_for_integration <= 0.0) {
    throw std::invalid_argument("max_dt_for_integration must be > 0");
  }
}

void PidController::reset(Real integrator) {
  st_ = PidState{};
  st_.integrator = integrator;
}

bool PidController::should_integrate(Real dt) const {
  if (!cfg_.freeze_integrator_on_large_dt) return true;
  return dt <= cfg_.max_dt_for_integration;
}

Telemetry PidController::step(Real setpoint, Real measurement, Real dt) {
  Telemetry out{};
  (void)step_impl(setpoint, measurement, dt, &out);
  return out;
}

Real PidController::step_u(Real setpoint, Real measurement, Real dt) {
  return step_impl(setpoint, measurement, dt, nullptr);
}

Real PidController::step_impl(Real setpoint, Real measurement, Real dt, Telemetry* out) {
  if (dt <= 0.0) {
    throw std::invalid_argument("dt must be > 0");
  }

  const Real error = setpoint - measurement;

  
  const Real p = cfg_.kp * error;

  
  Real d_raw = 0.0;
  if (st_.has_prev) {
    if (cfg_.derivative_on_measurement) {
      const Real d_meas = (measurement - st_.prev_measurement) / dt;
      d_raw = -cfg_.kd * d_meas;
    } else {
      const Real d_err = (error - st_.prev_error) / dt;
      d_raw = cfg_.kd * d_err;
    }
  }

  const Real d_filtered = d_lp_.step(d_raw, dt, st_.has_d_filt, st_.d_filtered);
  const Real d = d_filtered;

  
  const bool integrate_ok = should_integrate(dt);
  Real i_next = st_.integrator;
  if (integrate_ok) {
    i_next = st_.integrator + cfg_.ki * error * dt;
  }

  
  const Real u_unsat_candidate = p + i_next + d;
  const Real u_sat_candidate = clamp_output(u_unsat_candidate, cfg_.output_min, cfg_.output_max);
  const bool sat_candidate = is_saturated(u_unsat_candidate, u_sat_candidate);

  switch (cfg_.anti_windup) {
    case AntiWindupMode::NONE: {
      st_.integrator = i_next;
      break;
    }
    case AntiWindupMode::CLAMP: {
      const Real u_pd = p + d;
      const Real i_min = cfg_.output_min - u_pd;
      const Real i_max = cfg_.output_max - u_pd;
      st_.integrator = std::clamp(i_next, i_min, i_max);
      break;
    }
    case AntiWindupMode::CONDITIONAL_INTEGRATION: {
      if (!integrate_ok) {
        
      } else if (!sat_candidate) {
        st_.integrator = i_next;
      } else {
        const bool sat_high = (u_sat_candidate >= cfg_.output_max);
        const bool sat_low  = (u_sat_candidate <= cfg_.output_min);
        const bool helps = (sat_high && error < 0.0) || (sat_low && error > 0.0);
        if (helps) st_.integrator = i_next;
      }
      break;
    }
    case AntiWindupMode::BACK_CALCULATION: {
      if (!integrate_ok) {
        
      } else {
        const Real aw_err = (u_sat_candidate - u_unsat_candidate);
        st_.integrator = i_next + cfg_.backcalc_gain * aw_err;
      }
      break;
    }
    default:
      st_.integrator = i_next;
      break;
  }

  const Real i = st_.integrator;

  
  const Real u_unsat = p + i + d;
  Real u_sat = clamp_output(u_unsat, cfg_.output_min, cfg_.output_max);
  const bool saturated = is_saturated(u_unsat, u_sat);

  
  u_sat = rate_.step(u_sat, dt, st_.has_prev_u, st_.prev_u);
  u_sat = clamp_output(u_sat, cfg_.output_min, cfg_.output_max);

  
  st_.prev_error = error;
  st_.prev_measurement = measurement;
  st_.has_prev = true;

  
  if (out) {
    out->error = error;
    out->p = p;
    out->i = i;
    out->d = d;
    out->u_unsat = u_unsat;
    out->u = u_sat;
    out->saturated = saturated;
    out->integrator = i;
    out->d_filtered = st_.d_filtered;
  }

  return u_sat;
}

} 
