from __future__ import annotations

import sys
from dataclasses import dataclass
from typing import Literal, Optional, Tuple, Union

try:
    from . import _core
except Exception as e:
    raise ImportError(
        "openpid._core failed to import. "
        "If you're installing from source, ensure a C++ compiler is available and the build succeeded."
    ) from e


def dataclass_compat(**kwargs):
    if sys.version_info < (3, 10):
        kwargs.pop("slots", None)
    return dataclass(**kwargs)


AntiWindupMode = Literal["none", "clamp", "conditional_integration", "back_calculation"]


@dataclass_compat(frozen=True, slots=True)
class PIDConfig:
    kp: float
    ki: float
    kd: float

    output_min: float
    output_max: float

    anti_windup: AntiWindupMode = "conditional_integration"
    backcalc_gain: float = 1.0

    derivative_tau: float = 0.0
    derivative_on_measurement: bool = True

    rate_limit: float = 0.0

    freeze_integrator_on_large_dt: bool = True
    max_dt_for_integration: float = 0.5


@dataclass_compat(frozen=True, slots=True)
class Telemetry:
    error: float
    p: float
    i: float
    d: float
    u_unsat: float
    u: float
    saturated: bool
    integrator: float
    d_filtered: float


class PID:
    def __init__(self, config: PIDConfig):
        self._config = config
        self._ctrl = _core.PidController(self._to_core_config(config))
        self._last: Optional[Telemetry] = None

    @staticmethod
    def _to_core_config(cfg: PIDConfig) -> "_core.PidConfig":
        aw_map = {
            "none": _core.AntiWindupMode.NONE,
            "clamp": _core.AntiWindupMode.CLAMP,
            "conditional_integration": _core.AntiWindupMode.CONDITIONAL_INTEGRATION,
            "back_calculation": _core.AntiWindupMode.BACK_CALCULATION,
        }
        if cfg.anti_windup not in aw_map:
            raise ValueError(f"Unknown anti_windup mode: {cfg.anti_windup!r}")

        if not (cfg.output_min < cfg.output_max):
            raise ValueError("output_min must be < output_max")
        if cfg.derivative_tau < 0:
            raise ValueError("derivative_tau must be >= 0")
        if cfg.rate_limit < 0:
            raise ValueError("rate_limit must be >= 0")
        if cfg.max_dt_for_integration <= 0:
            raise ValueError("max_dt_for_integration must be > 0")

        c = _core.PidConfig()
        c.kp = float(cfg.kp)
        c.ki = float(cfg.ki)
        c.kd = float(cfg.kd)
        c.output_min = float(cfg.output_min)
        c.output_max = float(cfg.output_max)
        c.anti_windup = aw_map[cfg.anti_windup]
        c.backcalc_gain = float(cfg.backcalc_gain)
        c.derivative_tau = float(cfg.derivative_tau)
        c.derivative_on_measurement = bool(cfg.derivative_on_measurement)
        c.rate_limit = float(cfg.rate_limit)
        c.freeze_integrator_on_large_dt = bool(cfg.freeze_integrator_on_large_dt)
        c.max_dt_for_integration = float(cfg.max_dt_for_integration)
        return c

    @property
    def config(self) -> PIDConfig:
        return self._config

    def reset(self, integrator: float = 0.0) -> None:
        self._ctrl.reset(float(integrator))
        self._last = None

    @property
    def last(self) -> Optional[Telemetry]:
        return self._last

    def update(
        self,
        setpoint: float,
        measurement: float,
        dt: float,
        *,
        return_telemetry: bool = False,
    ) -> Union[float, Tuple[float, Telemetry]]:
        if dt <= 0:
            raise ValueError("dt must be > 0")

        if return_telemetry:
            t = self._ctrl.step(float(setpoint), float(measurement), float(dt))
            telem = Telemetry(
                error=t.error,
                p=t.p,
                i=t.i,
                d=t.d,
                u_unsat=t.u_unsat,
                u=t.u,
                saturated=t.saturated,
                integrator=t.integrator,
                d_filtered=t.d_filtered,
            )
            self._last = telem
            return telem.u, telem

        u = self._ctrl.step_u(float(setpoint), float(measurement), float(dt))
        return float(u)
