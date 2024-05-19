# mkdd
WIP 1:1 Decompilation of Mario Kart Double Dash!!
## Building
NOTE: Currently fails on Windows, use Linux in the meantime.
### Requirements
- [devkitPro](https://devkitpro.org/wiki/Getting_Started)
- [Ninja](https://github.com/ninja-build/ninja/releases/tag/v1.11.1)
- [Python 3.8+](https://www.python.org/downloads/release/python-3104/) 
- [GC MW Compilers](https://files.decomp.dev/compilers_latest.zip)

### Initial setup
- Install python, 3.10.4 is recommended as it gets installed by default on most recent Linux distros
- Install devkitPro, the only package you need is devkitPPC which gets installed by selecting either Wii or GameCube development
- Add the path where ninja is located to your environment variables under `PATH`
- Download the GC MW Compilers
- Clone the repo using `git clone https://github.com/SwareJonge/mkdd --recursive`
- Install the modules from requirements.txt (`pip install -r requirements.txt`)
- Dump a copy of the Debug or PAL version and extract `main.dol`
- For Debug place `main.dol` in `orig/MarioClub_us/`
- For PAL place `main.dol` in `orig/Release_eu/`
- Put the contents of the `GC` folder inside `compilers_latest.zip` in the `tools` folder

### Instructions
- To target Debug, run `python configure.py -r us`
- To target PAL, run `python configure.py -r eu`
- If `dol_slices.yml` changed, rerun the configure script
- Run `ninja`
- Last build step should say something like: ``main.dol: OK``
- Output is located in the `out` folder

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
