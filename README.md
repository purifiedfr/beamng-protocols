# BeamNG protocols
A server listening for [OutGauge and MotionSim](https://documentation.beamng.com/modding/protocols/) protocols, allowing you to display drivetrain and motion information.

###### This is just a simple prototype, not a finished implementation.

![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

## Features
* Prints the speed, RPM and current gear, as shown here:

![Screenshot of logging](https://github.com/purifiedfr/beamng-protocols/blob/main/screenshot.png?raw=true)
* Allows for easy extensibility with MotionSim

## TODO
* Add built-in support for communication with specific microcontrollers

## Usage
### To build:
```sh
$ cmake -DCMAKE_BUILD_TYPE=Release -B build .
$ cmake --build build --config Release
```
#### Or, download a release from [the Releases page](https://github.com/purifiedfr/beamng-protocols/releases)

### Then:
```sh
$ ./beamng-protocols <[optional port, default: 4444]>
```
