rd /s /q pkg
mkdir pkg
xcopy /e ..\resources pkg
xcopy /e ..\docs pkg\core\FTSE\docs
copy ..\release\FTSE.dll pkg\BinkW32.dll
cd pkg
tar -a -c -f ..\FTSE_0.60a.zip *
cd ..