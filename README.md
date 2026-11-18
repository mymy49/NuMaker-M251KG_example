# NuMaker-M251KG_example

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![GitHub Pages](https://img.shields.io/badge/docs-GitHub_Pages-blue.svg)](https://mymy49.github.io/NuMaker-M251KG_example/)

Firmware examples and Board Support Package (BSP) for the **Nuvoton NuMaker-M251KG** development board powered by **yss OS** (embedded C++ RTOS framework).

---

## Overview

This repository provides comprehensive sample projects demonstrating the peripherals and capabilities of the Nuvoton M251 series microcontroller (Arm® Cortex®-M23) using the object-oriented yss OS framework. Examples range from basic GPIO and UART communication to complex multi-tasking graphical user interface (GUI) applications with touch input and non-volatile flash storage.

### Key Features
- **yss OS Multitasking**: Preemptive thread scheduler, event loop, software timers, and task managers.
- **Graphical Display & Touch GUI**: ST7789V 2.8-inch TFT LCD controller driver with XPT2046 resistive touch screen interface, 2-point touch calibration, and interactive drawing canvas.
- **Hardware Acceleration**: High-speed SPI master communication and hardware-accelerated CRC32 checksum engine.
- **Non-volatile Configuration (Flash)**: Persistent storage of calibration data in internal on-chip flash memory with CRC integrity validation.
- **Peripheral Drivers**: EADC (Analog-to-Digital Converter), UART (asynchronous serial), PWM, and GPIO on-board LED control.

---

## Development Environment & Specifications

### Hardware Specifications
- **Target MCU**: Nuvoton M251KG6AE (Arm® Cortex®-M23, up to 48 MHz)
- **Target Board**: NuMaker-M251KG V1.1
- **Expansion Shield**: 2.8-inch TFT Touch Shield Rev2.1 (Arduino Uno compatible header)

### Pin Mapping

| Peripheral | Function / Signal | MCU Pin | Description |
|:---|:---|:---|:---|
| **User LED** | Red LED | `PB14` | Active-low on-board indicator |
| **UART0** | RXD | `PB12` | Serial Receive Data |
| **UART0** | TXD | `PB13` | Serial Transmit Data |
| **EADC0** | Channel 0 | `PB0` | Analog Input Channel 0 |
| **EADC0** | Channel 1 | `PB1` | Analog Input Channel 1 |
| **SPI0** | MOSI | `PA0` | SPI Master Output Slave Input |
| **SPI0** | MISO | `PA1` | SPI Master Input Slave Output |
| **SPI0** | CLK | `PA2` | SPI Serial Clock |
| **ST7789V LCD** | Chip Select (CS) | `PA3` | Display controller chip select |
| **ST7789V LCD** | Backlight (BL) | `PA4` | Display backlight enable |
| **ST7789V LCD** | Data/Command (DC) | `PA6` | Display data / command selector |
| **XPT2046 Touch** | Chip Select (CS) | `PC3` | Touch controller chip select |
| **XPT2046 Touch** | Interrupt (IRQ) | `PC5` | Touch event interrupt line |

### Toolchain & Dependencies
- **IDE / Toolchain**: SEGGER Embedded Studio for ARM (v8.x or later) or ARM GNU Toolchain
- **Operating System / Framework**: [yss OS](https://github.com/mymy49/yss)
- **Submodule Dependencies**:
  - `module/2.8inch_TFT_Touch_Shield_Rev2.1`
  - `module/ValueDisplay`
  - `module/Lpf`
  - `module/XPT2046`

---

## Sample Code Overview

The `SampleCode/` directory contains standalone projects configured for SEGGER Embedded Studio (`.hzp`):

| Project | Path | Description |
|:---|:---|:---|
| **2.8inch TFT Touch Shield** | `SampleCode/2.8inch TFT Touch Shield Rev2.1` | Full GUI demo including startup logo, 2-point touch calibration, and drawing application with color palette |
| **LED Blink** | `SampleCode/LED_blink` | Basic GPIO toggle demonstration using on-board red LED |
| **UART** | `SampleCode/UART` | Serial communication, interrupt-driven echo, and trigger-based LED activity indicator |
| **EADC** | `SampleCode/EADC` | Enhanced Analog-to-Digital Converter 2-channel data acquisition with digital filtering |
| **CRC** | `SampleCode/CRC` | Hardware-based CRC32 calculation and verification |
| **MutexWatchdog** | `SampleCode/MutexWatchdog` | Thread synchronization, deadlock detection, and watchdog timer management |
| **Runtime** | `SampleCode/runtime` | Execution time tracking and precision timer delays |

---

## Getting Started

### 1. Clone Repository with Submodules

To clone this repository together with all necessary submodules (yss OS and peripheral modules), run:

```bash
git clone --recursive https://github.com/mymy49/NuMaker-M251KG_example.git
```

If you have already cloned without submodules, initialize and update them:

```bash
cd NuMaker-M251KG_example
git submodule update --init --recursive
```

### 2. Build & Flash

1. Open **SEGGER Embedded Studio for ARM**.
2. Go to **File -> Open Solution...** and select the `.hzp` project file of interest, for example:
   ```text
   SampleCode/2.8inch TFT Touch Shield Rev2.1/Example.hzp
   ```
3. Connect the NuMaker-M251KG board to your computer via USB (Nu-Link-Me programming interface).
4. Select **Build -> Build Example** (`F7`).
5. Select **Debug -> Go** (`F5`) to flash and run the application on target hardware.

---

## Quick Start Examples

### Example 1: Basic GPIO & Threading (LED Blink)

```cpp
#include <yss.h>
#include <dev/led.h>

int main(void)
{
    // Initialize the yss OS kernel and system clock
    initializeYss();

    // Initialize the on-board red LED GPIO (PB14)
    Led::initialize();

    while (1)
    {
        // Turn ON the red LED (active-low)
        Led::red(true);
        thread::delay(500);

        // Turn OFF the red LED
        Led::red(false);
        thread::delay(500);
    }
}
```

### Example 2: Serial Communication Echo (UART0)

```cpp
#include <yss.h>
#include <bsp.h>

int main(void)
{
    initializeYss();
    initializeBoard(); // Configures UART0 at 115200 8N1

    while (1)
    {
        uint32_t count = uart0.getRxCount();

        if (count > 0)
        {
            int8_t *buf = uart0.getRxBuffer();
            uart0.send(buf, count);          // Echo received data
            uart0.releaseRxBuffer(count);   // Release ring buffer
        }
        else
        {
            thread::yield();
        }
    }
}
```

---

## Documentation & API Reference

Comprehensive Doxygen API documentation is automatically published on GitHub Pages:
- **Online Documentation**: [https://mymy49.github.io/NuMaker-M251KG_example/](https://mymy49.github.io/NuMaker-M251KG_example/)

To build the HTML documentation locally:
```bash
doxygen Doxyfile
```
The output files will be generated in `./html/index.html`.

---

## Author

- **Yoon-Ki Hong**

---

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
