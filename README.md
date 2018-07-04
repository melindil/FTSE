# FTSE
Fallout Tactics Scripting Engine (0.2a, July 2, 2018)

This mod enhances the game Fallout Tactics, adding the following capabilities:

## Configurable EXE patching
Most of the known hex-edits for the BOS executable file are included. Each can be selectively enabled or disabled from a configuration file. New edits can also be added without needing to directly edit the EXE, or rebuild the mod.

## Custom Scripting Engine
The mod will patch the BOS executable to load an included DLL, containing a Lua language interpreter, and interfaces to the Fallout Tactics data. The Lua code can specify code to execute at certain pre-defined trigger points within the game, and modify game variables in real-time to produce customized behaviors.

# Installation Instructions
Download the latest .ZIP release from the Release directory, and uncompress. Included is an installer EXE file - running this will prompt for the location of the Fallout Tactics EXE file. After selecting the EXE, the installer will patch it to load the included DLL at startup. A backup of the EXE will be produced.
Note that the installer will check the EXE to ensure that a) the patch is not yet installed, and b) that the code being modified is as expected (BOS.EXE version 1.27). Should this check fail, the EXE will not be modified.
After installation, double-check that the FTSE.DLL, FTSE-Config.json, and FTSE.lua files have been copied to the Fallout Tactics directory.
Upon starting the game, if everything is working, the FTSE version should be included in the version string near the bottom of the main menu screen.

More documentation will be forthcoming as new functionality is added.

Enjoy!
