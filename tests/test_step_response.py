from openpid import PID, PIDConfig

def test_step_response_moves_toward_setpoint():
    tau = 1.0
    dt = 0.01
    pid = PID(PIDConfig(kp=2.0, ki=1.0, kd=0.0, output_min=0.0, output_max=10.0))

    sp = 5.0
    y = 0.0
    for _ in range(800):
        u = pid.update(sp, y, dt)
        y += (u - y) * (dt / tau)

    assert y > 3.5
    assert y < 6.5
