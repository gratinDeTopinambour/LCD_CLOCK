# ⏱️ LCD_CLOCK

A complete **open-source low-power LCD clock project** based on the **STM32U083RCT6** microcontroller.

This repository contains all the resources required to build the project:

- 📐 **3D Design**: Enclosure and mechanical parts for 3D printing.
- 🔌 **Electronics**: Schematics, PCB layout, BOM, Gerbers, and PDF documentation.
- 💻 **Firmware**: Embedded C software developed using STM32CubeIDE / STM32CubeMX.
- 🖥️ **LCD Display**: Direct driving of a 4-digit LCD display by the STM32.
- ⏰ **Timekeeping**: Utilizes the internal RTC and low-frequency LSE oscillator.
- 🔋 **Power Management**: Leverages low-power peripherals and STOP2 sleep mode.
- 🌡️ **Analog Measurements**: Temperature and battery voltage monitoring via ADC.
- 📖 **Documentation**: Technical files and documents needed for manufacturing and development.

---

## 🖼️ Project Overview

| LCD Clock |
| :---: |
| ![LCD Clock](pictures/20260919_160751.jpg) |
| ![LCD Clock](pictures/20260919_161016.jpg) |

---

## 📂 Project Structure

```text
LCD_CLOCK/
│
├── 3D_design/                             # Mechanical design
│   ├── LCD_Clock_back.stl                 # Back part of the enclosure
│   ├── LCD_Clock_button_lumiere.stl       # Light control button
│   ├── LCD_Clock_button.stl               # Control button
│   ├── LCD_Clock_capot.stl                # Enclosure top cover
│   ├── LCD_Clock_dessous.stl              # Bottom part of the enclosure
│   ├── LCD_Clock_face_avant.stl           # Front panel
│   ├── LCD_Clock.3mf                      # Complete 3D project / model
│   └── LCD_Clock.step                     # STEP CAD model
│
├── Electronics/                           # Electronics design
│   ├── BOM/                               # Bill of Materials
│   ├── GERBER/                            # PCB manufacturing files
│   ├── KICAD/                             # KiCad project
│   └── PDF_version/                       # PDF electronics documentation
│
├── pictures/                              # Project photos
│   ├── 20260919_160751.jpg
│   └── 20260919_161016.jpg
│
├── Program/                               # STM32 firmware
│   ├── .settings/                         # STM32CubeIDE settings
│   ├── Core/                              # Application code and configuration
│   ├── Drivers/                           # Drivers and libraries
│   ├── .cproject                          # Eclipse project configuration
│   ├── .mxproject                         # STM32CubeMX configuration
│   ├── .project                           # STM32CubeIDE project file
│   ├── LCD_CLOCK Debug.launch             # Debug launch configuration
│   ├── LCD_CLOCK.ioc                      # STM32CubeMX configuration file
│   └── STM32U083RCTX_FLASH.ld             # Linker script
│
├── README.md                              # Main documentation
└── LICENSE                                # Project license

---

## ⚡ Technical Specifications

* **Microcontroller**: STM32U083RCT6 (Ultra-low-power ARM Cortex-M0+).
* **Display**: 4-digit LCD directly driven by the microcontroller.
* **Timekeeping**: RTC with LSE low-frequency oscillator.
* **Low Power**: Leverages RTC/LPTIM peripherals and STOP2 mode.
* **Measurements**: Temperature and battery voltage measured via ADC.
* **PCB Design**: Designed with **KiCad**, Gerber files and BOM included.
* **Power Supply**: Designed for battery operation.

---

## 🖨️ 3D Printed Enclosure

Parts are located in the `3D_design/` directory.

| Part | Format | Description |
| :--- | :--- | :--- |
| `LCD_Clock.3mf` | `.3mf` | 3D enclosure model |
| `LCD_Clock.step` | `.step` | CAD enclosure model |
| `LCD_Clock_capot.stl` | `.stl` | Top cover |
| `LCD_Clock_face_avant.stl` | `.stl` | Front panel |
| `LCD_Clock_back.stl` | `.stl` | Back part |
| `LCD_Clock_dessous.stl` | `.stl` | Bottom part |
| `LCD_Clock_button.stl` | `.stl` | Control button |
| `LCD_Clock_button_lumiere.stl` | `.stl` | Light control button |

---

## 💻 Firmware & Development

Source code is located in `Program/`.

### Prerequisites

* [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html) (recent version)
* Debugger / Programmer (ST-Link V2 / V3)

### Building and Flashing

1. Open **STM32CubeIDE**.
2. Import the project from `Program/`.
3. If you wish to modify peripheral configurations, open the `LCD_CLOCK.ioc` file.
4. Build (`Build Project`) then flash the microcontroller using the included debug configuration (`LCD_CLOCK Debug.launch`).

---

## 🏭 PCB Manufacturing

To manufacture the PCB:

1. Production-ready manufacturing files are located in `Electronics/GERBER/`.
2. The Bill of Materials (BOM) is available in `Electronics/BOM/`.
3. Original design files are available in `Electronics/KICAD/`.
4. Schematics and documentation are available in `Electronics/PDF_version/`.

---

## 📖 Documentation

Technical documentation for this project can be found in the `Electronics/PDF_version/` and `Program/` directories.

---

## 📜 License

This project is released under an open-source license. You are free to use, modify, and distribute it.
