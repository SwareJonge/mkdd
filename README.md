# mkdd
Decompilation of Mario Kart Double Dash!!
## Building
### Requirements
- DevkitPPC r38 or r41, other versions are not tested
> **Note**
> on windows the environment variables get set incorrectly for devkitpro, 
> change it to the path where you stored devkitPro
- Ninja(Optional: add ninja to your environment variables)
- Python 3.8+

### Instructions
- clone the repo using `git clone https://github.com/SwareJonge/mkdd --recursive`
- Dump a copy of the Debug or PAL version and extract  `main.dol`
- for Debug place  `main.dol` in `orig/MarioClub_us/`
- for PAL place  `main.dol` in `orig/Release_eu/`
- Place the compilers of GC MW 2.6 in `tools/2.6`
- Place the compilers of GC MW 1.3.2 in `tools/1.3.2`
- Place the compilers of GC MW 1.2.5 in `tools/1.2.5`
- (First time only) Install the modules from requirements.txt (`pip install -r requirementstxt`)
- To target Debug, run `python configure.py -r us`
- To target PAL, run `python configure.py -r eu`
- If `dol_slices.yml` changed, rerun the configure script
- Run `ninja`

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
