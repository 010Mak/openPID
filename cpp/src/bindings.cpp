#include <pybind11/pybind11.h>

#include "openpid/pid.hpp"
#include "openpid/pid_config.hpp"
#include "openpid/telemetry.hpp"

namespace py = pybind11;
using namespace openpid;

PYBIND11_MODULE(_core, m) {
  m.doc() = "openPID C++ core (pybind11)";

  py::enum_<AntiWindupMode>(m, "AntiWindupMode")
      .value("NONE", AntiWindupMode::NONE)
      .value("CLAMP", AntiWindupMode::CLAMP)
      .value("CONDITIONAL_INTEGRATION", AntiWindupMode::CONDITIONAL_INTEGRATION)
      .value("BACK_CALCULATION", AntiWindupMode::BACK_CALCULATION)
      .export_values();

  py::class_<PidConfig>(m, "PidConfig")
      .def(py::init<>())
      .def_readwrite("kp", &PidConfig::kp)
      .def_readwrite("ki", &PidConfig::ki)
      .def_readwrite("kd", &PidConfig::kd)
      .def_readwrite("output_min", &PidConfig::output_min)
      .def_readwrite("output_max", &PidConfig::output_max)
      .def_readwrite("anti_windup", &PidConfig::anti_windup)
      .def_readwrite("backcalc_gain", &PidConfig::backcalc_gain)
      .def_readwrite("derivative_tau", &PidConfig::derivative_tau)
      .def_readwrite("derivative_on_measurement", &PidConfig::derivative_on_measurement)
      .def_readwrite("rate_limit", &PidConfig::rate_limit)
      .def_readwrite("freeze_integrator_on_large_dt", &PidConfig::freeze_integrator_on_large_dt)
      .def_readwrite("max_dt_for_integration", &PidConfig::max_dt_for_integration);

  py::class_<Telemetry>(m, "Telemetry")
      .def(py::init<>())
      .def_readonly("error", &Telemetry::error)
      .def_readonly("p", &Telemetry::p)
      .def_readonly("i", &Telemetry::i)
      .def_readonly("d", &Telemetry::d)
      .def_readonly("u_unsat", &Telemetry::u_unsat)
      .def_readonly("u", &Telemetry::u)
      .def_readonly("saturated", &Telemetry::saturated)
      .def_readonly("integrator", &Telemetry::integrator)
      .def_readonly("d_filtered", &Telemetry::d_filtered);

  py::class_<PidController>(m, "PidController")
      .def(py::init<const PidConfig&>(), py::arg("config"))
      .def("reset", &PidController::reset, py::arg("integrator") = 0.0)
      .def("step", &PidController::step, py::arg("setpoint"), py::arg("measurement"), py::arg("dt"))
      .def("step_u", &PidController::step_u, py::arg("setpoint"), py::arg("measurement"), py::arg("dt"));
}
