# TwaiTaskBased Library - ESP32-C6

A FreeRTOS task-based wrapper for TWAI (Two-Wire Automotive Interface) CAN communication on ESP32-C6 microcontrollers.

## Overview

This library provides a clean abstraction for handling TWAI/CAN bus communication using FreeRTOS tasks, enabling efficient, non-blocking communication in multi-threaded ESP32 applications.

## Features

- **Task-Based Design**: Runs TWAI communication in separate FreeRTOS task
- **Non-Blocking**: Doesn't block main application logic
- **Thread-Safe**: Proper synchronization for concurrent access
- **Configurable**: Flexible setup for different CAN bus configurations
- **ESP32-C6 Optimized**: Designed for ESP32-C6 microcontroller

## Installation

### Using PlatformIO

Add to your `platformio.ini`:

```ini
lib_deps =
    https://codeberg.org/trailcurrent/Esp32C6TwaiTaskBasedLibrary.git@0.0.3
```

### Manual Installation

1. Clone or download this library
2. Copy to your project's `lib/` directory
3. Include in your sketch:

```cpp
#include <TwaiTaskBased.h>
```

## Quick Start

```cpp
#include <TwaiTaskBased.h>

void setup() {
  Serial.begin(115200);

  // Initialize TWAI with task-based handling
  // (Configuration details in header file)
  TwaiTaskBased::init();
}

void loop() {
  // Your application code here
  // TWAI communication runs in background task
  delay(1000);
}
```

## Configuration

See `include/TwaiTaskBased.h` for detailed configuration options including:
- CAN bus speed (250kbps, 500kbps, 1Mbps, etc.)
- GPIO pin assignments
- Task priority and stack size
- Filter settings

## Hardware Requirements

- ESP32-C6 microcontroller
- CAN transceiver module (SN65HVD230, etc.)
- Appropriate GPIO pins for TWAI TX/RX

## License

MIT License - See LICENSE file for details

## Contributing

Improvements and contributions are welcome! Please submit issues or pull requests to the repository.
