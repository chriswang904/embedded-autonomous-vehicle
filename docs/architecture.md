# Architecture

## Stage 1: FreeRTOS / K66

`freertos_hello.c` initializes the board clocks, motor component, RC receiver and LEDs, then starts the FreeRTOS scheduler. Terminal and accelerometer initialization calls are commented out.

| Component | Observed responsibility |
| --- | --- |
| RC receiver | UART1 at 115200 baud; reads a frame beginning with 0x20 and checks header 0x4020; maps RC channels to speed, direction, steering and mode |
| Motor control | Separate motor and steering tasks consume queues and update FTM0 PWM channel registers |
| LED | A queue-driven task selects red, blue or green to indicate the speed mode |
| Terminal | Queue-based UART output code is present, but setup and control paths are incomplete and disabled in main |
| Accelerometer | Placeholder functions; not an implemented sensing pipeline in this snapshot |

The active initialization paths create four tasks: RC, motor, steering and LED. This snapshot does not establish a seven-task implementation.

## Stage 2: PX4 and Raspberry Pi

| File | Role |
| --- | --- |
| `hello_world_step0.cpp` | Prints RC channel values via a uORB subscription |
| `hello_world_part1.cpp` | Maps RC input to motor and servo commands via `test_motor` |
| `hello_world_part2.cpp` | Reads direction/speed from `debug_value` and publishes motor/servo commands |
| `hello_world.py` | Processes camera edges, measures ultrasonic echo time, sends direction/speed via MAVLink DEBUG |

The Python program selects a direction from a simple three-region edge-distance heuristic. It uses BCM GPIO 23 (trigger), GPIO 24 (echo), camera index 0 and `/dev/ttyACM0`. These values must be checked against the actual wiring and operating system.

Outbound DEBUG fields are constructed as `(time_boot_ms=0, ind=direction, value=speed)`. The PX4 example reads `debug_data.ind` as direction and `debug_data.value` as speed. The inbound print labels in Python appear reversed relative to this mapping. The Python receive call blocks for DEBUG, while the PX4 echo publication is after an infinite loop; a working reply path is not established by these files alone.
