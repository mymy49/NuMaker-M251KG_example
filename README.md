# NuMaker-M251KG_example

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![GitHub Pages](https://img.shields.io/badge/docs-GitHub_Pages-blue.svg)](https://mymy49.github.io/NuMaker-M251KG_example/)

Firmware examples and Board Support Package (BSP) for the **NuMaker-M251KG** development board, providing full peripheral driver support for the **Nuvoton M251 series** microcontroller powered by **yss OS** (embedded C++ RTOS framework).

---

## 1. Overview

This repository hosts official example applications and board support drivers for the Nuvoton NuMaker-M251KG evaluation board. It demonstrates how to leverage the **Nuvoton M251 Support Package** integrated into yss OS to build efficient, multitasking embedded applications for the Arm® Cortex®-M23 architecture.

---

## 2. Nuvoton M251 Support Package

The Nuvoton M251 Support Package provides a comprehensive Hardware Abstraction Layer (HAL) and MCU Support Package (MSP) tailored for the Nuvoton NuMicro® M251 series microcontrollers. It encapsulates low-level hardware registers into modern, object-oriented C++ driver instances.

### Supported Peripherals & Drivers

- **Clock & Power Management (`clock`)**:
  - Flexible system clock generation supporting both Internal High-Speed RC oscillator (HIRC at 48 MHz) and External Crystal (HXT, 4 to 24 MHz).
  - On-chip Phase-Locked Loop (PLL) configuration supporting up to 48 MHz system core clock (HCLK).
  - Dynamic peripheral clock prescaling and routing for APB0 and APB1 buses.

- **GPIO & Pin Multiplexing (`gpioA`, `gpioB`, `gpioC`, `gpioD`, `gpioE`, `gpioF`)**:
  - Independent pin direction control (Input, Push-Pull Output, Open-Drain, Quasi-bidirectional).
  - Peripheral alternate function routing (`setAsAltFunc()`).
  - High-speed bit-level reading and writing.

- **UART Communication (`uart0`, `uart1`, `uart2`)**:
  - Asynchronous serial communication up to high baud rates (e.g., 115200, 921600 bps).
  - Interrupt-driven transmission and reception with configurable ring buffer.
  - Integration with yss OS asynchronous event triggers.

- **Enhanced ADC (`eadc`)**:
  - 14-bit successive approximation register (SAR) Analog-to-Digital Converter.
  - Multi-channel simultaneous conversion and digital low-pass filtering (LPF) support.
  - Continuous non-blocking background conversions.

- **Hardware CRC Engine (`crc`)**:
  - Hardware-accelerated CRC-CCITT, CRC-8, CRC-16, and CRC-32 polynomials.
  - Programmable seed values, data lengths (8, 16, 32 bits), and bit/byte reversal options.

- **Timers & Runtime (`timer0`, `timer1`, `timer2`, `timer3`)**:
  - Periodic interrupt generation and event timing.
  - OS tick generation and high-resolution elapsed time measurement (`runtime::getMsec()`).

- **Flash Memory Controller (`flash`)**:
  - In-System Programming (ISP) and In-Application Programming (IAP) driver.
  - Page-based erase and 32-bit word programming for non-volatile parameter persistence.

- **Watchdog Timer & RTOS Mutex Watchdog (`wdt`, `Mutex`)**:
  - Hardware independent watchdog timer supervision.
  - Automatic software mutex lock duration monitoring and deadlock prevention.

- **Serial Peripheral Interface (`spi0`) & USCI (`uspi0`, `uuart0`)**:
  - Master and slave SPI modes with programmable clock polarities and phases.
  - Universal Serial Communication Interface support.

### System Configuration (`config.h`)

Each application configures the M251 Support Package via `app/inc/config.h`:
- Enabling or disabling peripheral driver instances to minimize code footprint.
- Selecting the runtime measurement timer (`YSS_RUNTIME_TIMER`).
- Configuring thread scheduling parameters (`THREAD_GIVEN_CLOCK`, `MAX_THREAD`).
- Enabling Mutex Watchdog threshold limits (`THREAD_WATCHDOG_ENABLE`, `THREAD_WATCHDOG_OVERFLOW_TIME`).

---

## 3. Development Environment & Specifications

### Hardware Specifications
- **Target Evaluation Board**: NuMaker-M251KG V1.1
- **Target Microcontroller**: Nuvoton M251KG6AE
  - **Core**: Arm® Cortex®-M23 (Armv8-M Baseline architecture)
  - **Max Clock Speed**: Up to 48 MHz
  - **Flash Memory**: 256 KB on-chip APROM
  - **SRAM**: 32 KB embedded SRAM
  - **Operating Voltage**: 1.75 V to 5.5 V
- **On-Board Debugger**: Nu-Link-Me (SWD programming and Virtual COM port interface)

### Toolchain & Software Dependencies
- **IDE / Toolchain**: CrossWorks for ARM (v5.x or higher)
- **Operating System / Framework**: [yss OS](https://github.com/mymy49/yss) (embedded C++ RTOS)

---

## 4. Prerequisites: Nuvoton M251 Support Package for CrossWorks for ARM

CrossWorks for ARM does not natively provide device support for Nuvoton NuMicro® M251 microcontrollers in its default package manager. A custom device support package must be installed beforehand:

1. Clone or download the [CrossworksForARM_Nuvoton_M251](https://github.com/mymy49/CrossworksForARM_Nuvoton_M251) repository.
2. Open the package project file (`.hzp`) in CrossWorks for ARM and build it (`F7` or `Build -> Build Project`) to generate the package file (`.hzq`).
3. In CrossWorks for ARM, navigate to **Tools -> Packages -> Manually Install Packages...** and select the generated `.hzq` package file.
4. Once installed, M251 target device definitions, CMSIS headers, and project templates will be fully available in CrossWorks for ARM.

---

## 5. Sample Code Overview

The `SampleCode/` directory contains complete projects configured with solution files (`.hzp`):

| Project | Path | Focus Area |
|:---|:---|:---|
| **LED Blink** | `SampleCode/LED_blink` | Basic GPIO configuration and non-blocking RTOS thread delays |
| **UART** | `SampleCode/UART` | Serial communication, interrupt-driven reception, and asynchronous triggers |
| **EADC** | `SampleCode/EADC` | 14-bit dual-channel ADC acquisition with digital filtering |
| **CRC** | `SampleCode/CRC` | Hardware CRC32 calculation and checksum verification |
| **MutexWatchdog** | `SampleCode/MutexWatchdog` | Thread synchronization and mutex deadlock supervision |
| **Runtime** | `SampleCode/runtime` | High-precision system uptime and execution timing |
| **2.8inch TFT Touch Shield** | `SampleCode/2.8inch TFT Touch Shield Rev2.1` | Multi-tasking application integrating display, touch, and flash storage |

---

## 6. Getting Started & Build Instructions

### 1. Clone Repository with Submodules

To clone this repository along with yss OS and all dependencies, run:

```bash
git clone --recursive https://github.com/mymy49/NuMaker-M251KG_example.git
```

If the repository was cloned without submodules, initialize them using:

```bash
cd NuMaker-M251KG_example
git submodule update --init --recursive
```

### 2. Build and Firmware Flashing

#### Using CrossWorks for ARM :
1. Ensure the [CrossworksForARM_Nuvoton_M251](https://github.com/mymy49/CrossworksForARM_Nuvoton_M251) package has been installed.
2. Select **File -> Open Solution...** and choose the desired project file (e.g., `SampleCode/LED_blink/Example.hzp`).
3. Select target configuration (`Example THUMB Release` or `Example THUMB Debug`).
4. Build solution via `F7` or `Build -> Build Solution`.
5. Connect the NuMaker-M251KG board via Nu-Link-Me USB ICE interface and flash with `F5` / `Ctrl+D` (`Target -> Download`).

---

## 7. Quick Start Examples

### 1. GPIO Control & Thread Delay

```cpp
#include <yss.h>
#include <dev/led.h>

int main(void)
{
    // Initialize yss OS kernel and M251 clock system
    initializeYss();

    // Initialize on-board LED GPIO
    Led::initialize();

    while (1)
    {
        Led::red(true);       // Turn LED ON
        thread::delay(500);   // Non-blocking OS delay

        Led::red(false);      // Turn LED OFF
        thread::delay(500);
    }
}
```

### 2. Interrupt-Driven UART Communication

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
            uart0.send(buf, count);          // Echo back received bytes
            uart0.releaseRxBuffer(count);   // Release ring buffer bytes
        }
        else
        {
            thread::yield();
        }
    }
}
```

### 3. Dual-Channel Analog Data Acquisition (EADC)

```cpp
#include <yss.h>
#include <bsp.h>
#include <yss/debug.h>

int main(void)
{
    initializeYss();
    initializeBoard(); // Configures EADC channels with LPF

    while (1)
    {
        // Read 14-bit converted results from Channel 0 and Channel 1
        int32_t ch0 = eadc.getResult(0);
        int32_t ch1 = eadc.getResult(1);

        debug_printf("CH0: %d, CH1: %d\n", ch0, ch1);
        thread::delay(200);
    }
}
```

---

## 8. Documentation & API Reference

Comprehensive Doxygen API documentation is published on GitHub Pages:
- **Online Documentation**: [https://mymy49.github.io/NuMaker-M251KG_example/](https://mymy49.github.io/NuMaker-M251KG_example/)

To build the documentation locally:
```bash
doxygen Doxyfile
```
Documentation will be generated in `./html/index.html`.

---

## 9. Author

- **Yoon-Ki Hong**

---

## 10. License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
