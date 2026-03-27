# Installation Instructions

## New installation
Download the latest .ZIP release from the Release directory, and uncompress into the Fallout Tactics game directory. If prompted, overwrite the binkw32.dll file with the one from the archive - this DLL includes the FTSE functionality. The original binkw32.dll is included in the archive as realBinkw32.dll.

## Upgrade installation

**IMPORTANT NOTE:** Starting with 0.60a, FTSE now uses a DLL wrapper to inject the FTSE functionality into the game process, rather than a patch to the executable. As such, the FTSESetup.exe setup process is no longer necessary. It is recommended to follow the below cleanup steps prior to upgrading from release 0.56a or earlier:

* Make a backup copy of FTSE_config.json, ftse.lua, and any other Lua scripts that may already be in place. For larger mods, it may be a good idea to make a full backup copy first.
* Run the original FTSESetup.exe from the currently installed version of FTSE, and select the option to uninstall the EXE patch.
* Remove the FTSE.DLL file from the game directory.

Once ready, uncompress the new release ZIP file into the Fallout Tactics game directory, as with a new installation. The FTSE configuration is now written to the "core\FTSE" subdirectory, so any previous FTSE\_config.json file and any Lua scripts (including ftse.lua, but **not** ftse\_base.lua) should be copied there.

If a mod uses the -path command when starting the game, then it is required to also make a copy of the full FTSE directory under "core" in the specified path directory.

## Validating installation
Once the installation is complete, run Fallout: Tactics.  If the installation is correct, the FTSE version number should show along with the Fallout: Tactics version in the bottom left corner of the main menu. In the default configuration, there should also be two new buttons at the main menu: Editor Tools and FTSE Config. The Editor Tools option unlocks the 1.27 version of the campaign, level and entity editing tools (same as with FT Improver). The FTSE Config option allows selection of hex patches to apply to the game (as was previously configured in FTSESetup.exe).
