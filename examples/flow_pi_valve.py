from openpid import PID, PIDConfig

def main():
    k = 1.0
    tau = 0.7
    dt = 0.02

    pid = PID(PIDConfig(
        kp=4.0, ki=2.0, kd=0.0,
        output_min=0.0, output_max=100.0,
        anti_windup="conditional_integration",
        rate_limit=200.0,
    ))

    sp = 10.0
    q = 0.0
    for i in range(500):
        u, t = pid.update(sp, q, dt, return_telemetry=True)
        q += (k * (u/100.0) - q) * (dt / tau)
        if i % 50 == 0:
            print(f"t={i*dt:5.2f}s  q={q:6.3f}  u={u:6.2f}  P={t.p:6.3f} I={t.i:6.3f} sat={t.saturated}")

if __name__ == "__main__":
    main()
