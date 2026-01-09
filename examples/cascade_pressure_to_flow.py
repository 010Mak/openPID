from openpid import PID, PIDConfig

def main():
    dt = 0.02

    outer = PID(PIDConfig(
        kp=1.5, ki=0.8, kd=0.0,
        output_min=0.0, output_max=20.0,
        anti_windup="conditional_integration",
        rate_limit=50.0
    ))

    inner = PID(PIDConfig(
        kp=3.0, ki=2.0, kd=0.05,
        output_min=0.0, output_max=100.0,
        anti_windup="conditional_integration",
        derivative_tau=0.05,
        rate_limit=300.0
    ))

    flow = 0.0
    pressure = 0.0

    flow_tau = 0.3
    pressure_tau = 1.2

    sp_pressure = 1.0

    for i in range(800):
        sp_flow = outer.update(sp_pressure, pressure, dt)
        valve = inner.update(sp_flow, flow, dt)

        flow += ((valve / 100.0) - flow) * (dt / flow_tau)
        pressure += ((1.8 * flow) - pressure) * (dt / pressure_tau)

        if i % 80 == 0:
            print(f"t={i*dt:5.2f}s  P={pressure:6.3f}  F={flow:6.3f}  spF={sp_flow:6.2f}  valve={valve:6.2f}")

if __name__ == "__main__":
    main()
