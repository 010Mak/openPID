from openpid import PID, PIDConfig

def main():
    k = 2.0
    tau = 1.5
    dt = 0.05

    pid = PID(PIDConfig(
        kp=3.0, ki=1.2, kd=0.0,
        output_min=0.0, output_max=1.0,
        anti_windup="clamp",
        rate_limit=0.5,
    ))

    sp = 1.0
    p = 0.0
    for i in range(300):
        u, t = pid.update(sp, p, dt, return_telemetry=True)
        p += (k * u - p) * (dt / tau)
        if i % 25 == 0:
            print(f"t={i*dt:5.2f}s  p={p:6.3f}  u={u:6.3f}  I={t.i:6.3f} sat={t.saturated}")

if __name__ == "__main__":
    main()
