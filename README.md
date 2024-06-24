Mario Kart Double Dash!!
[![Build Status]][actions] ![Progress]
=============

[Build Status]: https://github.com/SwareJonge/mkdd/actions/workflows/build.yml/badge.svg
[actions]: https://github.com/SwareJonge/mkdd/actions/workflows/build.yml
[Progress]: https://img.shields.io/endpoint?label=Code&url=https%3A%2F%2Fprogress.decomp.club%2Fdata%2Fmkdd%2FMarioClub_us%2Fdol%2F%3Fmode%3Dshield%26measure%3Dcode

WIP 1:1 Decompilation of Mario Kart Double Dash!!

This repository does **not** contain any game assets or assembly whatsoever. An existing copy of the game is required.

Currently only the debug version is supported

## Building
### Requirements
- [Ninja](https://github.com/ninja-build/ninja/releases/tag/v1.11.1)
- [Python 3.8+](https://www.python.org/downloads/release/python-3104/) 

### Initial setup
- Install python, 3.10.4 is recommended as it gets installed by default on most recent Linux distros
- Add the path where ninja is located to your environment variables under `PATH`
- Clone the repository using `git clone https://github.com/SwareJonge/mkdd`
- Dump a copy of the Debug version and extract `main.dol` and put it in `orig/MarioClub_us/` 

### Instructions
- run `python configure.py`
- run `ninja`

## Credits
- [Seeky](https://github.com/SeekyCt) for ppcdis, without it this project wouldn't have been where it is now
### Matching
Used code from the following repositories(JSystem, SDK)
* [Metroid Prime Decomp](https://github.com/PrimeDecomp/prime)
* [Pikmin 2 Decomp](https://github.com/projectPiki/pikmin2)
* [Super Mario Galaxy Decomp](https://github.com/shibbo/Petari)
* [Super Mario Sunshine Decomp](https://github.com/doldecomp/sms)
* [Syati](https://github.com/PMArkive/Syati-pretransfer)
* [Twilight Princess Decomp](https://github.com/zeldaret/tp)

And many thanks to the people who helped me match some game functions(sorry if i forgot someone)
* [Chippy](https://github.com/1superchip)
* [kiwi](https://github.com/kiwi515)
* [Seeky](https://github.com/SeekyCt)
