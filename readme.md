# Security Pattern Lock for STM32F429

This project implements a smartphone-style pattern lock screen for the STM32 microcontroller.

## 🌟 Key Features
- **Lock Screen:** Draw a pattern to unlock the device. Displays error feedback upon incorrect attempts.
- **Security Lockout:** Automatically locks the system temporarily after 5 consecutive incorrect attempts.
- **Register New Pattern:** Allows users to set a new pattern by drawing and confirming it.
- **Non-volatile Storage:** The pattern is securely saved into Flash memory (Sector 23 at address `0x081E0000`), ensuring the password is retained even after power cycles.
- **Hardware Trigger:** Press and hold the USER button (PA0) on the STM32F429I-DISC1 kit for 3 seconds to trigger the pattern change screen.

## 🛠 Hardware & Software
- **Hardware:** `STM32F429I-DISC1` Development Kit (Built-in 2.4" TFT LCD).
- **Software:** 
  - [TouchGFX Designer 4.25.0](https://www.st.com/en/development-tools/touchgfxdesigner.html) (GUI Design)
  - [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html) (C/C++ Compilation and Flashing)
  - STM32CubeMX (Hardware Configuration)

## 🚀 How to Run
1. Clone this repository.
2. Open the `.ioc` file with **STM32CubeMX** and generate code if necessary.
3. Open the `.part` project file with **TouchGFX Designer** and click `Run Simulator` to test the GUI on your PC.
4. Navigate to the `STM32CubeIDE/` directory and double-click `.project` to open the workspace in the IDE.
5. Build and flash the firmware to the physical board using the `Run/Debug` button.