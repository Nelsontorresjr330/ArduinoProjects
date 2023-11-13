# Arduino Nano Snake Game
Welcome to the Arduino Nano Snake Game! This README provides you with the basic information to get started with your very own Snake game on an Arduino Nano.

## Overview
The Snake Game is a classic arcade game where the player maneuvers a line which grows in length, with the line itself being a primary obstacle. This version is designed to run on an Arduino Nano, offering a simple and fun way to get acquainted with electronics and programming.

## Getting Started
Parts Required
To build this project, you will need the following parts:

- [Arduino Nano](https://www.amazon.com/Arduino-Nano-Every-Single-Board/dp/B07VX7MX27/ref=sr_1_4?crid=2M7QZNT62KL6M&keywords=arduino%2Bnano&qid=1699898460&sprefix=arduino%2Bnano%2Caps%2C77&sr=8-4&th=1)
- Breadboard
- [OLED display module](https://www.amazon.com/BAEASU-Display-Self-Luminous-Compatible-Raspberry/dp/B0BYYNB3G5/ref=sr_1_4?crid=1PZETYODOSS1N&keywords=breadboard%2BOLED%2Bdisplay&qid=1699898419&sprefix=breadboard%2Boled%2Bdisplay%2Caps%2C71&sr=8-4&th=1)
- [Joystick module](https://www.amazon.com/WMYCONGCONG-Joystick-Controller-Multicolored-Breadboard/dp/B07KZDDFKR/ref=sr_1_4?crid=CO9R065CUGMD&keywords=breadboard+joystick&qid=1699898384&sprefix=breadboard+joy+stick%2Caps%2C78&sr=8-4) 
- Jumper wires
- (Optional) [9V Batteries](https://www.amazon.com/LP-Rechargeable-Battery-Charger-Pack/dp/B07RT332DY/ref=sr_1_2_sspa?crid=2HYFNOGHJ8DXC&keywords=rechargeable%2B9v%2Bbatteries%2Bwith%2Bcharger&qid=1699898310&s=electronics&sprefix=9v%2Bbatteries%2Brecha%2Celectronics%2C72&sr=1-2-spons&sp_csd=d2lkZ2V0TmFtZT1zcF9hdGY&th=1) and [Adapter](https://www.amazon.com/outstanding-Cocodile-Alligator-Battery-Connector/dp/B084RNF2WT/ref=sr_1_3?crid=32EIJW2BY341L&keywords=breadboard+alligator+clips+9v&qid=1699898215&sprefix=breadboard+alligator+clips+9v%2Caps%2C75&sr=8-3)

## Assembly
Attach the Arduino Nano to the breadboard.
Connect the OLED display module to the Arduino. (Pins A6 and A7)
Connect the Joysticks (My code uses pins A0, A1 and A2)
Ensure all connections are secure. Attach proper VCC and GRND Connections.

Note: Detailed electrical schematics will be available soon.

## Installation
Download the [Snake Game code](https://github.com/Nelsontorresjr330/ArduinoProjects/blob/main/SnakeGame/SnakeOnNano.ino) from the repository.
Open the code in the [Arduino IDE](https://www.arduino.cc/en/software).
Connect the Arduino Nano to your computer via USB.
Upload the code to the Arduino Nano using the Arduino IDE.

## Gameplay
Once the code is uploaded, the game will start automatically.
Use the Joystick to control the direction of the snake on the display.
The pause menu (Push in Joystick while in game) allows for the user to edit some settings.

## Pictures
![Physical Circuit](https://github.com/Nelsontorresjr330/ArduinoProjects/blob/main/SnakeGame/PhysicalCircuit.jpeg)
![Welcome Screen](https://github.com/Nelsontorresjr330/ArduinoProjects/blob/main/SnakeGame/MainScreen.jpeg)

## Notes
The code is open-source and can be modified to suit your preferences.
Be careful when handling electronic components.
The electrical schematics will give you a better understanding of the connections.
