<div align="center">
	<img src="https://github.com/user-attachments/assets/6caa9e4a-cc1c-4eb9-880c-17c982627205" alt="Re47 Logo">
</div>

<div align="center">
  <h1>Re47</h1>
</div>

[![](https://dcbadge.limes.pink/api/server/https://discord.gg/bcaWWF9Tb5)](https://discord.gg/bcaWWF9Tb5)

### Hitman Codename 47 (2000) - Decompilation

This repository contains the decompilation project for Hitman Codename 47 (2000). The goal is to reconstruct the source code from the original game binaries in a way that it can be compiled into a working executable. This project is an effort for game preservation.

## Socials

Join the discord server using the badge above to track the progress or if you just enjoy Hitman games in general!

## Status

Below each of the files in the decomp is discussed along with a percentage indicator of the progress.

### Completed ✅

**directplay.dll** [48kb]
**100%** Completed - **22** out of **22** functions named.

**globals.dll** [20kb]
**100%** Completed - **1** out of **1** functions named.

<br>

### Being Decompiled 📝

**hitman.exe** [45kb]
**99%** Completed - **3** out of **3** functions named.

**system.dll** [272kb]
**20%** Completed - **525** out of **552** functions named.

**systemprobedll.dll** [68kb]
**99%** Completed - **53** out of **53** functions named.

**renderopengl.dll** [248kb]
**0%** Completed - **240** out of **464** functions named.

<br>

### Untouched ❌

**hitmandlc.dlc** [2,496kb]
**0%** Completed

**enginedata.dll** [244kb]
**0%** Completed

**locale.dll** [92kb]
**0%** Completed

**renderd3d.dll** [272kb]
**0%** Completed

**render3dfx.dll** [212kb]
**0%** Completed

**sound.dll** [188kb]
**0%** Completed

## Prerequisites

To build and run the decompiled version of Hitman Codename 47, you will need:

-   CMake
-   The portable version of Visual Studio 6 SP2 - https://github.com/itsmattkc/MSVC600/tree/17bae53fa330df7b576f25f1f604e4214076a650
-   Visual Studio Code or an equivalent

## Building the Project

1. **Clone the repository:**
    ```bash
    git clone https://github.com/0danny/re47.git
    cd re47
    ```
2. **Set the VS6_BIN_PATH Environment Variable:**

    On windows navigate to "Edit the system environment variables" in the start menu. Click "Environment Variables" and under "System Variables" add a new entry with the name "VS6_BIN_PATH" and a value of your bin folder location (e.g. C:\VS6\VC98\Bin)

3. **Run build.bat**

    The output files should be written to the "build" folder at the root directory.

## Reversing

IDA Pro is being used to reverse the games executables and dlls, an IDA server could be something that is used to allow collaboration in the future. Currently it is just a guess that Hitman CN47 actually uses Visual Studio 6 for compilation given the time period.
It is MSVC, however which version and what flags were used during the compilation remains a mystery. A byte-accurate decomp would be nice, however that is a very timely process. The GOG version of the game is being used for the decomp as the original
contains SafeDisc V2.0 DRM and GOG is DRM free. We don't have access to any debug symbols at this current point in time.

## Contributing

Contributions to the Hitman Codename 47 decompilation project are welcome (and needed)! Here's how you can contribute:

### Naming Rules

-   Functions: `PascalCase`
-   Variables: `camelCase. Prefix with g_ if global, m_ for member variables or p_ for parameters.`
-   Classes: `PascalCase`
-   Files and Directories: `lowercase`
-   Namespaces: `PascalCase`

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
