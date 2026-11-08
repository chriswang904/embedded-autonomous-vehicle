# Validation and limitations

## Checks performed during consolidation

| Check | Result |
| --- | --- |
| Source preservation | All included source/configuration files match their original ZIP bytes by SHA-256 |
| Python syntax | Parsed successfully with Python ast; no hardware imports or execution |
| Repository organization | Relative documentation links and expected source files checked |
| Firmware compilation | Not performed; SDK/PX4 build environments absent |
| Hardware test / HIL | Not performed during consolidation |

## Known limitations from code inspection

- Accelerometer functions are stubs; terminal initialization and control paths are incomplete and disabled in main.
- RC parsing does not establish full-frame checksum validation or loss-of-signal timeout handling.
- Ultrasonic echo polling and MAVLink receive are blocking without timeouts.
- Camera read failures are not checked before frame processing. Fewer than three edge chunks can cause indexing errors; no-edge frames can produce a missing direction value.
- Camera geometry includes hard-coded coordinates, so the heuristic is tied to the original image assumptions.
- PX4 sample messages are not fully initialized, and message freshness checks/fault handling are incomplete.
- Companion receive depends on a DEBUG reply; the example echo publication is unreachable in its normal infinite loop. Integration must resolve this before claiming a working request/reply cycle.
- No demonstrated watchdog, communication-loss neutral action or validated safe-stop behavior is established by these snapshots.
- No new performance, reliability, AI-assistance or production-readiness claims are made.

## Proposed bench validation — not executed

1. Verify clocks, UART settings, pin mux and PWM frequency with the matching board configuration.
2. Check RC framing, malformed input and input-loss behavior with actuator drive disabled.
3. Measure PWM neutral/endpoints and check motor/servo response against the actual ESC/servo calibration.
4. Validate mode changes and queue behavior under repeated RC input.
5. Resolve MAVLink response handling; exercise missing, delayed and invalid messages.
6. Exercise distance boundaries at 15 and 50 cm, missing echo, disconnected camera and low-feature frames.
7. Record equipment, firmware revision, observations and pass/fail evidence for each check.
