# Setup and integration

This document records integration requirements, not a verified clean-machine build recipe. Exact original SDK, PX4 and toolchain versions must be recovered before claiming reproducibility.

## Stage 1

1. Obtain the matching NXP K66 board SDK and its FreeRTOS example environment.
2. Restore the board-specific `board.h`, `clock_config.h`, `pin_mux.h`, startup code, linker script and peripheral drivers referenced by these sources.
3. Integrate the supplied `Sources` files into that environment. Match `FreeRTOSConfig.h` to the selected kernel and SDK.
4. Check pin multiplexing, UART clock source, FTM clock and actuator calibration against the physical board.
5. Build and flash with the board's supported IDE/toolchain and probe. Record the actual versions and commands once verified.

The source snapshots omit enough build inputs that a generic standalone CMake file would not establish a working build. None has been invented here.

## Stage 2

1. Recover the PX4/NuttX revision and board target used for the project.
2. Integrate one C++ example at a time into a PX4 module named/configured consistently with its `hello_world_main` entry point. Module CMake files and board registration are not provided.
3. Check `test_motor`, `rc_channels` and `debug_value` message APIs against that revision. Configure the appropriate MAVLink DEBUG-to-uORB handling.
4. On Raspberry Pi, provide Python 3, `pymavlink`, `RPi.GPIO`, OpenCV and NumPy. Package versions and deployment OS were not included in the archives.
5. Confirm camera access, serial permissions, GPIO wiring and voltage compatibility before attempting execution. Restore the response path and address the limitations in the validation notes first.

Python package names are recorded in `companion/raspberry-pi/requirements.txt` as unpinned dependency inventory, not a tested lockfile. No firmware or hardware execution is claimed by this guide.
