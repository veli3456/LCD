# ATmega328P LCD & 7-Segment Counter

A bare-metal C driver for the HD44780 LCD (4-bit mode) and a 7-segment display, running on an ATmega328P microcontroller. 

## Overview
This project synchronizes a 16x2 LCD and a 7-segment display to act as a 0-9 counter.  

## Key Features
* **Bare-Metal C:** Direct register manipulation using `avr-gcc` without relying on heavy external libraries like Arduino.
* **Hardware Busy Flag Polling:** Reads the `D7` pin with precise Enable strobes and RS/RW control to dynamically check if the LCD is ready for the next instruction.
* **Synchronized Display:** The 7-segment display increments simultaneously with the text output on the LCD screen.

## Technologies Used
* ATmega328P Microcontroller
* HD44780 16x2 LCD
* 7-Segment Display