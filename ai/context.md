# knx_platformio

This project is a KNX library for the ESP32/ESP8266 hardware platform using PlatformIO.
It creates a library published for platformIO.

## Components

the library itself exists of files in the src and include directories.
The library.json file is used to define the library and its dependencies.
The library is published to the platformIO library repository.

The library doesn't require a platformio.ini.

The CI/CD pipeline runs on github upon push to the master/main branch.

Examples are part of the library in the examples directory.
The platformio.ini in the examples references the library in the lib_deps section using a symlink.
The examples are built with the local development version of the library, not the published version.

## Instructions

Always, ALWAYS create complete files when makeing changes - never EVER truncate a file.

Update the README.md whenever necessary to reflect the changes in the library.

Do not change the LICENSE file, it is used to define the license of the library.
Do not change the CHANGELOG.md file, it is used to keep track of the changes in the library by the CI/CD pipeline.

To use the platformIO tools like pio, make sure to activate platformIO's virtual environment 
by running `source ~/.platformio/penv/bin/activate` in the terminal.

I want you to keep track of the things you do in the journal directory in a per day journal file in markdown format.