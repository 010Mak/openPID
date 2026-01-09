# openPID

PID controller with a fast backend. Built for simulation and process-control style loops where you pass an explicit `dt`.

## install

```bash
pip install openpid
```

## quick start

```python
from openpid import PID, PIDConfig

pid = PID(PIDConfig(
    kp=2.0, ki=0.5, kd=0.1,
    output_min=0.0, output_max=100.0,
))

dt = 0.02
setpoint = 10.0
measurement = 0.0

u = pid.update(setpoint, measurement, dt)  # fast path (no telemetry object)
print(u)
```

## some telemetry

```python
u, t = pid.update(setpoint, measurement, dt, return_telemetry=True)
print(t.p, t.i, t.d, t.u, t.saturated)
```

## features

- Explicit `dt` 
- Output limits 
- Anti-windup modes (none / clamp / conditional integration / back-calculation)
- Derivative on exact measurement
- OPTIONAL derivative low-pass filter (`derivative_tau`)
- OPTIONAL output rate limiting (`rate_limit`)

## dev

```bash
pip install -e .
python -m pytest -q
```

## License

MIT
