# pompo

## Requirements

Before you get started, ensure you have the following:

1. **Segger Embedded Studio**: To build and upload the project, you need Segger Embedded Studio version 5.42a. You can download it from the official website:
   [Download Segger Embedded Studio 5.42a](https://www.segger.com/downloads/embedded-studio/Setup_EmbeddedStudio_ARM_v542a_win_x64.exe)

2. **nRF5 SDK**: To run the project, you need nRF5_SDK_17.1.0. You can download it from the following link:
   [Download nRF5 SDK 17.1.0](https://nsscprodmedia.blob.core.windows.net/prod/software-and-other-downloads/sdks/nrf5/binaries/nrf5_sdk_17.1.0_ddde560.zip)

   After downloading, make sure to extract the contents and place the SDK at `nRF5_SDK_17.1.0/examples/Workspace`.

## Getting Started

Follow these steps to build and run the example program:

1. **Install Segger Embedded Studio**:
   - Download Segger Embedded Studio version 5.42a from the link provided above.
   - Follow the installation instructions for your operating system.

2. **Set up nRF5 SDK**:
   - Clone the nRF5_SDK_17.1.0 repository to your local machine.
   - Ensure the repository is located at `nRF5_SDK_17.1.0/examples/Workspace`.

3. **Open the Project**:
   - Navigate to the project folder: `nRF5_SDK_17.1.0/examples/Workspace/pompo/pca10040/blank/twi_sensor_pca10040.emProject`.
   - Launch Segger Embedded Studio.

## Note

- This example assumes you are using a development board compatible with the `pca10040` board definition in the nRF5 SDK.
