import pytest
from openpid import PID, PIDConfig

def test_dt_must_be_positive():
    pid = PID(PIDConfig(kp=1.0, ki=0.0, kd=0.0, output_min=-1.0, output_max=1.0))
    with pytest.raises(ValueError):
        pid.update(1.0, 0.0, 0.0)
    with pytest.raises(ValueError):
        pid.update(1.0, 0.0, -0.1)
