# Installation Instructions

Download the latest .ZIP release from the Release directory, and uncompress. Included is FTSESetup.exe, and its required files.

## New installation
Running FTSESetup.exe will bring up a small UI. Select the BOS.EXE file, and click "Install". The setup utility will patch the EXE and copy all necessary files to the game directory. A backup of the EXE will be produced.

## Upgrade installation
Run the FTSESetup.exe executable, as in the normal install process. FTSESetup should automatically detect the BOS executable (assuming it is being run from the same location as previously - if not, select the BOS.EXE file as above). The new version of the DLL and configuration files will be written.

### Notes for upgrading from versions prior to 0.45a
If upgrading from an older release, it is necessary to remove any existing ftse.lua or ftse\_config.json files from the Fallout Tactics directory.  The default information normally in these files is now in a separate pair of configuration files.

## Manual installation
If FTSESetup.exe will not work (e.g. due to a lack of .NET runtime), the previous installation method may be used.

* Download FTSEInstaller.exe from the Release directory in the GitHub repository.
* Run FTSEInstaller.exe, pointing it to the BOS.EXE file. A dialog box should appear indicating the patch has been applied.
* Copy the latest FTSE.DLL file from the Release directory in the GitHub repository to the Fallout Tactics directory
* Copy the ftse\_base.lua and FTSE\_config\_base.json files from the FTSESetup directory in the GitHub repository to the Fallout Tactics directory.
* Make a second copy of the FTSE\_config\_base.json file named FTSE\_config.json.  Modify this file to turn on/off desired EXE patches.

## Validating installation
Once the installation is complete, run Fallout: Tactics.  If the installation is correct, the FTSE version number should show along with the Fallout: Tactics version in the bottom left corner of the main menu.
