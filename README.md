# BeamNG protocols
A simple server listening for [OutGauge and MotionSim](https://documentation.beamng.com/modding/protocols/) protocols from [BeamNG.drive](https://www.beamng.com/).
###### This is just a simple prototype, not a finished implementation or anything.

## TODO
* Add support for Windows
* Add built-in support for communication with specific microcontrollers

## Features
* Prints the speed, RPM and current gear, as shown here:
![Screenshot of logging](https://github.com/purifiedfr/beamng-protocols/blob/main/screenshot.png?raw=true)
* Allows for extensibility with MotionSim

## Usage:
To build: `sh ./build.sh` (Assumes g++ 11.4 or higher)

To run: `./beamng-protocols <[optional port, default: 4444]>`
