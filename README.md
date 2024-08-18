# BeamNG protocols
![Banner](https://github.com/purifiedfr/beamng-protocols/blob/main/banner.png?raw=true)
A simple server listening for [OutGauge and MotionSim](https://documentation.beamng.com/modding/protocols/) protocols from [BeamNG.drive](https://www.beamng.com/).
###### This is just a simple prototype, not a finished implementation or anything.

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

## TODO
* Add built-in support for communication with specific microcontrollers

## Features
* Prints the speed, RPM and current gear, as shown here:

![Screenshot of logging](https://github.com/purifiedfr/beamng-protocols/blob/main/screenshot.png?raw=true)
* Allows for extensibility with MotionSim

## Usage:
### To build (in Release):
```sh
$ cmake -DCMAKE_BUILD_TYPE=Release -B build .
$ cmake --build build --config Release
```
##### Note: Leave out the `--config Release` part if you're not using a generator that supports configurations, such as Visual Studio

### Or, download a release from [the Releases page](https://github.com/purifiedfr/beamng-protocols/releases)

### To run:
```sh
$ ./beamng-protocols <[optional port, default: 4444]>
```
