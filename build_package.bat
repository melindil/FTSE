rem msbuild /t:Rebuild /p:Configuration=Release
cd FTSESetup/bin/Release
powershell -File ../../../build_zip.ps1 ../../../Release/FTSE_%1%.zip
cd ../../..