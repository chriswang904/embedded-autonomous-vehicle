# Embedded Autonomous Vehicle Platform

A two-stage university team project exploring embedded vehicle control: **FreeRTOS firmware on an NXP K66 ARM Cortex-M4**, followed by **PX4/uORB control with a Raspberry Pi perception companion**.

The repository brings the two stages together for review while retaining their separate firmware environments. Source files are preserved byte-for-byte from the supplied project archives; this is an archival portfolio consolidation, not a newly validated release.

## What the code demonstrates

| Stage | Implementation | Main concepts |
| --- | --- | --- |
| 1 — MCU control | C, FreeRTOS, NXP K66 | UART RC parsing, task priorities, queues, motor/servo PWM, LED speed-mode feedback |
| 2 — System integration | C++ PX4 examples and Python on Raspberry Pi | uORB subscriptions/publications, MAVLink DEBUG messages, ultrasonic distance sensing, OpenCV edge-based steering heuristic |

Stage 1 initializes motor, RC receiver and LED components before starting the scheduler. RC commands flow through queues to motor and steering tasks. PWM initialization uses NXP SDK calls, while duty-cycle updates directly write FTM channel registers.

Stage 2 contains alternative PX4 examples for inspecting RC input, controlling actuators from RC input, and accepting companion-computer commands. They share the `hello_world_main` entry point and must be integrated **one at a time**. They are not a single combined executable.

## Repository guide

| Path | Contents |
| --- | --- |
| `firmware/freertos-k66/Sources/` | Original Stage 1 C sources, headers and FreeRTOS configuration |
| `firmware/px4/examples/` | Original Stage 2 PX4 C++ variants |
| `companion/raspberry-pi/hello_world.py` | Camera processing, ultrasonic sensing and MAVLink messaging |
| [Architecture](docs/architecture.md) | Control paths and observed message mapping |
| [Setup and integration](docs/setup.md) | Required environments and missing build dependencies |
| [Validation and limitations](docs/validation.md) | Inspection results, known limitations and proposed bench checks |
| [Provenance](docs/provenance.md) | Team-project context and source preservation |

## Distance-based control

The companion program maps ultrasonic distance to a speed command:

| Measured distance | Speed code | PX4 example actuator value |
| --- | --- | --- |
| At or below 15 cm | 0 | 0.5, intended neutral |
| Above 15 cm and at or below 50 cm | 1 | 0.6 |
| Above 50 cm | 2 | 0.8 |

These are source-code mappings. Actuator neutral, direction and limits depend on the original hardware calibration and have not been rechecked during consolidation.

## Build and test status

The archives do not include a complete standalone NXP SDK or PX4 build tree, board configuration, toolchain versions, or all integration files. No firmware build, flashing or hardware execution was performed during this consolidation. The Python source was syntax-checked without importing hardware dependencies.

The accelerometer component and parts of the terminal component are placeholders. The companion program also needs robustness improvements before unattended operation; see [validation notes](docs/validation.md).

## Team context

Originally developed as McMaster COMPENG 4DS4 Group 2 coursework. This portfolio copy is maintained by Chris Wang and includes team-produced code. Individual module authorship is not established by the ZIP snapshots. See [provenance](docs/provenance.md) before attributing individual contributions or reusing the code.
