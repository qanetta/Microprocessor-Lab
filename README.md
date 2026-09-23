# Microprocessor and Microcontroller Lab (HCMUT)

This repository contains source code, simulation designs, and technical reports for the **Microprocessor - Microcontroller** course at Ho Chi Minh City University of Technology (HCMUT). The coursework focuses on embedded systems programming using the ARM Cortex-M (STM32F103C6/C8) architecture, STM32CubeIDE (HAL Library), and Proteus simulation.

---

## 📌 Course Outline (Lab 1 – Lab 4)

* **Lab 1: GPIO & LED Interfacing**  
  Fundamental GPIO configuration in output mode: multi-color LED sequencing, digital traffic light systems, 7-segment display decoding, and analog clock simulation using an active-low 12-LED array.
* **Lab 2: Timers & Interrupts**  
  Timebase generation using hardware timers and software timer flags. Multi-digit 7-segment display multiplexing, LED matrix display control, and precise periodic task execution without blocking delays (`HAL_Delay`).
* **Lab 3: Finite State Machine (FSM) & Input Handling**  
  Button debouncing algorithms, single/double/long-press event handling, and FSM-driven systems (e.g., configurable multi-mode traffic lights with real-time parameter tuning).
* **Lab 4: Real-Time Cooperative Scheduler**  
  Design and implementation of a non-preemptive / cooperative task scheduler for embedded platforms (task queuing, dispatcher, execution dispatching, and deterministic time tracking).

---

## 📂 Representative Repository Structure

All lab directories follow a unified and modular organization. The structure of **Lab 1** represents the general layout:

```text
Microprocessor-Lab/
├── lab1/
│   ├── proteus_files/          # Proteus schematic designs (.pdsprj)
│   ├── stm32cube_src/          # STM32CubeIDE firmware projects (HAL-based)
│   │   └── <Exercise_Name>/
│   │       ├── Core/           # Inc/ and Src/ (main.c, peripheral drivers)
│   │       ├── Debug/          # Compilation outputs (.hex, .bin, .elf)
│   │       └── ...             # IDE configuration (.ioc, .project)
│   └── report/                 # Lab report documentation (LaTeX source & PDF)
├── lab2/
├── lab3/
├── lab4/
└── README.md

```

---

## 🛠️ Tech Stack & Prerequisites

* **Hardware Target:** STM32F103 (ARM Cortex-M3 core)
* **Framework:** STM32Cube HAL (Hardware Abstraction Layer)
* **IDE / Toolchain:** STM32CubeIDE / GCC ARM Embedded Toolchain
* **Simulation:** Proteus Design Suite (v8.13+)
* **Documentation:** LaTeX / TeX Live (for formal technical reports)

---
* **Tran Minh Quang** — Ho Chi Minh City University of Technology (HCMUT)

```

```
