# FTSE
Fallout Tactics Scripting Engine (0.56a, August 15, 2024)

This mod enhances the game Fallout Tactics, adding the following capabilities:

## Configurable EXE patching
Most of the known hex-edits for the BOS executable file are included. Each can be selectively enabled or disabled from a configuration file. New edits can also be added without needing to directly edit the EXE, or rebuild the mod.

## Custom Scripting Engine
The mod will patch the BOS executable to load an included DLL, containing a Lua language interpreter, and interfaces to the Fallout Tactics data. The Lua code can specify code to execute at certain pre-defined trigger points within the game, and modify game variables in real-time to produce customized behaviors.

# Installation Instructions
Download the latest .ZIP release from the Release directory, and uncompress. Included is FTSESetup.exe - running this will bring up a small UI. Select the BOS.EXE file, and click "Install". The setup utility will patch the EXE and copy all necessary files to the game directory. A backup of the EXE will be produced.

Note that the installer will check the EXE to ensure that a) the patch is not yet installed, and b) that the code being modified is as expected (BOS.EXE version 1.27). If the UI shows "Installed", then the EXE has already been patched. The setup utility will still copy the latest versions of the DLL and config files.

If the UI shows "Unknown", then the EXE doesn't match the expected version, and will not allow patching.

The new UI also allows selecting individual hex patches. Select/unselect desired patches, then click "Apply Changes".

After installation, double-check that the FTSE.DLL, FTSE_config-base.json, and ftse_base.lua files have been copied to the Fallout Tactics directory.

Upon starting the game, if everything is working, the FTSE version should be included in the version string near the bottom of the main menu screen.

More documentation at: https://melindil.github.io/FTSE/manual/01-introduction.md.html

Enjoy!
