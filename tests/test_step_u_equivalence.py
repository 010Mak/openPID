from openpid import PID, PIDConfig

def test_step_u_matches_step_output_over_sequence():
    cfg = PIDConfig(
        kp=3.0, ki=1.0, kd=0.5,
        output_min=0.0, output_max=10.0,
        derivative_on_measurement=True,
        derivative_tau=0.02,
        rate_limit=5.0,
        anti_windup="conditional_integration",
    )

    pid_step = PID(cfg)
    pid_fast = PID(cfg)

    dt = 0.01
    sp = 0.0

    for k in range(300):
                                                              
        if k == 50:
            sp = 20.0
        if k == 200:
            sp = 5.0

        measurement = 0.1 * k                             
        u_step, _t = pid_step.update(sp, measurement, dt, return_telemetry=True)
        u_fast = pid_fast.update(sp, measurement, dt)

        assert abs(u_step - u_fast) < 1e-12
