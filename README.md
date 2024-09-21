# Hitman Codename 47 (2000) Decompilation

This repository contains the decompilation project for Hitman Codename 47 (2000). The goal is to reconstruct the source code from the original game binaries in a way that it can be compiled into a working executable. This project is an effort for game preservation.

## Socials
Join the discord server and track the progress! - https://discord.gg/bcaWWF9Tb5

## Status
Below each of the files in the decomp is discussed along with a percentage indicator of the progress.

### Files
1. **hitman.exe** (80%) - Imports ZSysInterface from Globals.dll and calls some sort of init function in WinMain.
2. **globals.dll** (100%) - Exports global instances of each class type used ZSysInterface, ZSysMem etc.
3. **system.dll** (10%) - Implements each class type exported by globals.dll.
4. **directplay.dll** (0%)
5. **enginedata.dll** (0%)
6. **locale.dll** (0%)
7. **renderd3d.dll** (0%)
8. **renderopengl.dll** (0%)
6. **render3dfx.dll** (0%)
6. **sound.dll** (0%)
6. **systemprobedll.dll** (0%)

Currently hitman.exe functions as original besides a couple of edge cases, globals.dll also acts exactly as original.

## Prerequisites

To build and run the decompiled version of Hitman Codename 47, you will need:

- CMake
- The portable version of Visual Studio 6 - https://github.com/itsmattkc/MSVC600
- Visual Studio Code or an equivalent

## Building the Project

1. **Clone the repository:**
   ```bash
   git clone https://github.com/<your-username>/hitman-codename-47.git
   cd hitman-codename-47
   ```
2. **Set the VS6_BIN_PATH Environment Variable:**
   
   On windows navigate to "Edit the system environment variables" in the start menu. Click "Environment Variables" and under "System Variables" add a new entry with the name "VS6_BIN_PATH" and a value of your bin folder location (e.g. C:\VS6\bin)
   
4. **Run build.bat**

   The output files should be written to the "build" folder at the root directory.

This project is in very early stages, a more streamlined build process is a future task.

## Reversing

IDA Pro is being used to reverse the games executables and dlls, an IDA server could be something that is used to allow collaboration in the future. Currently it is just a guess that Hitman CN47 actually uses Visual Studio 6 for compilation given the time period. 
It is MSVC, however which version and what flags were used during the compilation remains a mystery. A byte-accurate decomp would be nice, however that is a very timely process. The GOG version of the game is being used for the decomp as the original
contains SafeDisc V2.0 DRM and GOG is DRM free. We don't have access to any debug symbols at this current point in time.

## Contributing

Contributions to the Hitman Codename 47 decompilation project are welcome (and needed)! Here's how you can contribute:

### Naming Rules
* Functions: `PascalCase`
* Variables: `lower_case_underscored. Prefix with g_ if global or m_ for member variables.`
* Classes: `PascalCase`
* Files and Directories: `lower_case_underscored`
* Namespaces: `PascalCase`

### Rules/Notes

1. Please try to match the functionality as closely as possible to the original game. This is currently not a byte-accurate decomp so you don't have to go overboard if something doesn't match exactly.
2. Do not introduce any external dependencies into the project without consulting someone first.
3. Do not include any retail resources from the original game in your pull requests.

**Reporting bugs**: Use the Issues tab to report any bugs or inconsistencies.

**Submitting fixes**: Submit pull requests with descriptions of the problem and solution.

**Enhancing documentation**: Improvements to README, code comments etc are much appreciated.

Join the discord for more information.

## Disclaimer
This project is for educational and game preservation purposes only and is not for commercial use. The repository does not contain any game assets/retail resources or proprietary code from the original game. The files built using this repository may only be utilized with assets provided by ownership of Hitman Codename 47 (2000). This project is in no way associated with or endorsed by IO Interactive.
