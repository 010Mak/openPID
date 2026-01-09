from openpid import PID, PIDConfig

def test_derivative_on_measurement_reduces_setpoint_kick():
    cfg = PIDConfig(
        kp=0.0, ki=0.0, kd=1.0,
        output_min=-100.0, output_max=100.0,
        derivative_on_measurement=True,
        derivative_tau=0.02,
    )
    pid = PID(cfg)
    dt = 0.01

    y = 0.0
    pid.update(0.0, y, dt)
    u1, t1 = pid.update(10.0, y, dt, return_telemetry=True)

    assert abs(t1.d) < 1e-6
    assert abs(u1) < 1e-6
