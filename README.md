# Keypad to Frequency Converter

This project is a keypad to frequency converter implemented using the 8051 microcontroller. It allows the user to input a number using a keypad, and then converts that number into a corresponding frequency output. The generated frequency is used to toggle an output pin on the microcontroller.

## Code Overview

The code provided is written in C and is targeted for the 8051 microcontroller. It includes the following main components:

- Keypad Scanning: The code scans a 4x3 keypad to detect key presses and retrieves the corresponding key value.
- Frequency Calculation: Upon receiving a key press, the code performs calculations to determine the corresponding frequency output. It uses a series of if-else statements to handle different key values and apply specific frequency conversion formulas.
- Timer Setup: The code initializes Timer 1 to generate the desired frequency output. It sets the appropriate register values for the timer based on the calculated frequency.
- Interrupt Handling: The code includes an interrupt service routine to toggle the output pin at the specified frequency. It uses Timer 1 interrupts to control the output toggling.
- Seven-Segment Display: The code implements a simple function to display the current input number on a four-digit seven-segment display.

## Prerequisites

To run this code, you need the following:

- An 8051 microcontroller board with a keypad and a seven-segment display connected.
- A compatible development environment, such as Keil uVision, to compile and upload the code to the microcontroller.
- Required components and connections as per the circuit diagram.

## Usage

1. Set up the 8051 microcontroller board with the keypad and seven-segment display connected.

2. Open the provided code in your preferred development environment.

3. Build and upload the code to the microcontroller.

4. Power on the microcontroller and observe the seven-segment display.

5. Press the keys on the keypad to input numbers. The corresponding frequency will be displayed on the seven-segment display.

6. Connect an output device, such as an LED, to the specified output pin. The LED will toggle at the generated frequency.

## Circuit Diagram

Insert a circuit diagram here to illustrate the connections between the microcontroller, keypad, seven-segment display, and output device.

## Contributing

Contributions to this project are welcome! If you have any ideas or improvements, feel free to submit a pull request. Make sure to follow the existing code style and guidelines.

## License

This project is licensed under the [MIT License](LICENSE).

