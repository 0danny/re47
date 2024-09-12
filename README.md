# Hitman Codename 47 (2000) Decompilation

This repository contains the decompilation project for Hitman Codename 47 (2000). The goal is to reconstruct the source code from the original game binaries in a way that it can be compiled into a working executable. This project is an effort for game preservation.

## Status
Below each of the files in the decomp is discussed along with a percentage indicator of the progress.

### Files
1. **hitman.exe** (80%) - Imports ZSysInterface from Globals.dll and calls some sort of init function in WinMain.
2. **Globals.dll** (20%) - Exports global instances of each class type used ZSysInterface, ZSysMem etc.
3. **system.dll** (1%) - Responsible for building the window/drawing etc.

These are the only files being reversed as of now. Currently the plan is to get hitman.exe working as original and stubbing all of the system.dll functions so that we can run our version with the original dlls in place to confirm functionality.

## Prerequisites

To build and run the decompiled version of Hitman Codename 47, you will need:

- CMake (version 3.5 or higher)
- The portable version of Visual Studio 6 (See build.bat) - https://github.com/itsmattkc/MSVC600
- Visual Studio Code

## Building the Project

1. **Clone the repository:**
   ```bash
   git clone https://github.com/<your-username>/hitman-codename-47.git
   cd hitman-codename-47
   ```
2. **Set your VS6/Bin path inside of build.bat**
   ```bash
   @echo off
   set VS6_BIN_PATH=C:\VS6\bin <-- HERE
   ```
3. **Run build.bat**

This project is in very early stages and currently I am the only person working on it, a more streamlined build process is a future task.

## Reversing

IDA Pro is being used to reverse the games executables and dlls, an IDA server could be something that is used to allow collaboration in the future. Currently it is just a guess that Hitman CN47 actually uses Visual Studio 6 for compilation. 
I can confirm it is MSVC, however which version and what flags were used during the compilation remains a mystery. A byte-accurate decomp would be nice, however that is a very timely process. The GOG version of the game is being used for the decomp as the original
contains SafeDisc V2.0 DRM.

## Socials

There is currently no discord or anything setup, however if you would like to message me, you can at 0.dan on discord.

## Contributing

Contributions to the Hitman Codename 47 decompilation project are welcome (and needed)! Here's how you can contribute:

**Reporting bugs**: Use the Issues tab to report any bugs or inconsistencies.

**Submitting fixes**: Submit pull requests with descriptions of the problem and solution.

**Enhancing documentation**: Improvements to README, code comments etc are much appreciated.

It is currently just me working on this project, any help is needed.

## Disclaimer
This project is for educational purposes only. The repository does not contain any game assets or proprietary code from the original game.
