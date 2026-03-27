# FTSE
Fallout Tactics Scripting Engine (0.60a, March 27, 2026)

This mod enhances the game Fallout Tactics, adding the following capabilities:

## Configurable EXE patching
Most of the known hex-edits for the BOS executable file are included. Each can be selectively enabled or disabled from a configuration file. New edits can also be added without needing to directly edit the EXE, or rebuild the mod.

## Custom Scripting Engine
The mod will patch the BOS executable to load an included DLL, containing a Lua language interpreter, and interfaces to the Fallout Tactics data. The Lua code can specify code to execute at certain pre-defined trigger points within the game, and modify game variables in real-time to produce customized behaviors.

# Installation Instructions (NEW for 0.60a)
Download the latest .ZIP release from the Release directory, and uncompress the files into the Fallout Tactics game directory, replacing any existing files (specifically binkw32.dll). 

Upon starting the game, if everything is working, the FTSE version should be included in the version string near the bottom of the main menu screen.

Changes to the set of installed hex patches are now managed via the "FTSE Config" option displayed on the Fallout Tactics main menu. This replaces the interface previously offered in FTSESetup.

More documentation at: https://melindil.github.io/FTSE/manual/01-introduction.md.html

Enjoy!
