from openpid import PID, PIDConfig

def test_antiwindup_prevents_integrator_runaway_under_saturation():
    cfg = PIDConfig(
        kp=0.0, ki=10.0, kd=0.0,
        output_min=0.0, output_max=1.0,
        anti_windup="conditional_integration"
    )
    pid = PID(cfg)

    dt = 0.01
    sp = 10.0
    y = 0.0

    for _ in range(500):
        pid.update(sp, y, dt)

                                                                         
    u_last, last = pid.update(sp, y, dt, return_telemetry=True)

    assert last.integrator < 5.0
    assert 0.0 <= u_last <= 1.0
    assert 0.0 <= last.u <= 1.0
