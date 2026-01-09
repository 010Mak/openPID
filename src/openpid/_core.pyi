from __future__ import annotations
from dataclasses import dataclass
from enum import Enum

class AntiWindupMode(Enum):
    NONE: int
    CLAMP: int
    CONDITIONAL_INTEGRATION: int
    BACK_CALCULATION: int

@dataclass
class PidConfig:
    kp: float
    ki: float
    kd: float
    output_min: float
    output_max: float
    anti_windup: AntiWindupMode
    backcalc_gain: float
    derivative_tau: float
    derivative_on_measurement: bool
    rate_limit: float
    freeze_integrator_on_large_dt: bool
    max_dt_for_integration: float

@dataclass
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

class PidController:
    def __init__(self, config: PidConfig) -> None: ...
    def reset(self, integrator: float = 0.0) -> None: ...
    def step(self, setpoint: float, measurement: float, dt: float) -> Telemetry: ...
    def step_u(self, setpoint: float, measurement: float, dt: float) -> float: ...
