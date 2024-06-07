rem msbuild /t:Rebuild /p:Configuration=Release
cd FTSESetup/bin/Release
7z a -tzip ../../../Release/FTSE_%1%.zip FTSESetup.exe FTSESetup.exe.config Newtonsoft.Json.dll
cd ../../..