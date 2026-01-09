from openpid import PID, PIDConfig

def test_slew_rate_limits_output_change():
    cfg = PIDConfig(
        kp=100.0, ki=0.0, kd=0.0,
        output_min=0.0, output_max=100.0,
        rate_limit=10.0,
    )
    pid = PID(cfg)
    dt = 0.1

    y = 0.0
    u0 = pid.update(0.0, y, dt)
    u1 = pid.update(100.0, y, dt)

    assert (u1 - u0) <= 1.0000001
