# pompo
Project Name: TWI Sensor Example

Requirements:

- Segger Embedded Studio version 5.42a is required to build and upload the project. You can download it from the official website: https://www.segger.com/downloads/embedded-studio/Setup_EmbeddedStudio_ARM_v542a_win_x64.exe

- To run the project, you need to have nRF5_SDK_17.1.0 installed on your system. Make sure to locate this repository at nRF5_SDK_17.1.0/examples/Workspace.

Running the Example Program:

1. Install Segger Embedded Studio version 5.42a by following the installation instructions provided in the link above.

2. Clone the nRF5_SDK_17.1.0 repository to your local machine, and make sure you have placed it at nRF5_SDK_17.1.0/examples/Workspace.

3. Locate the project folder in the repository: nRF5_SDK_17.1.0/examples/Workspace/pompo/pca10040/blank/twi_sensor_pca10040.emProject

4. Open Segger Embedded Studio.

5. In the Segger Embedded Studio IDE, go to File > Open and browse to the location of the project file twi_sensor_pca10040.emProject.

6. Build the project by clicking on the Build button or by pressing Ctrl + B.

7. Once the build process is successful, you can upload the program to your nRF52 development board.

8. Connect your nRF52 development board to your computer via a suitable programming/debugging interface.

9. In Segger Embedded Studio, click on the Target drop-down menu and select your connected nRF52 development board.

10. Click on the Download and Debug button or press Ctrl + D to upload and run the program on your board.

11. The example program will now be running on your nRF52 development board, and you should see the expected behavior of the TWI sensor functionality.

Please note that this example assumes you have a development board compatible with the pca10040 board definition in the nRF5 SDK. If you have a different board, you may need to adjust the project settings accordingly.

For more information on nRF5_SDK_17.1.0 and its usage, refer to the official documentation provided by Nordic Semiconductor.

Happy coding!
