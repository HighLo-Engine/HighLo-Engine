@ECHO off

pushd ..

if exist HighLoEdit\bin\Debug-windows-x86_64\HighLoEdit\ (
	ECHO copying into HighLoEdit debug folder...
	ROBOCOPY HighLo\vendor\openssl\lib\ HighLoEdit\bin\Debug-windows-x86_64\HighLoEdit\ libcrypto-3-x64.dll /mt /z > nul
	ROBOCOPY HighLo\vendor\openssl\lib\ HighLoEdit\bin\Debug-windows-x86_64\HighLoEdit\ libssl-3-x64.dll /mt /z > nul
	ROBOCOPY HighLo\vendor\assimp\lib\Debug\ HighLoEdit\bin\Debug-windows-x86_64\HighLoEdit\ assimp-vc142-mtd.dll /mt /z > nul
	ROBOCOPY HighLo\assets\ HighLoEdit\bin\Debug-windows-x86_64\HighLoEdit\ editorconfig.ini /mt /z > nul
	ROBOCOPY HighLo\assets\ HighLoEdit\bin\Debug-windows-x86_64\HighLoEdit\assets\ /MIR > nul
	ROBOCOPY HighLoEdit\assets\ HighLoEdit\bin\Debug-windows-x86_64\HighLoEdit\assets\ /MIR > nul
	ECHO Copy done!
	ECHO.
)

if exist HighLoEdit\bin\Release-windows-x86_64\HighLoEdit\ (
	ECHO Copying into HighLoEdit release folder...
	ROBOCOPY HighLo\vendor\openssl\lib\ HighLoEdit\bin\Release-windows-x86_64\HighLoEdit\ libcrypto-3-x64.dll /mt /z > nul
	ROBOCOPY HighLo\vendor\openssl\lib\ HighLoEdit\bin\Release-windows-x86_64\HighLoEdit\ libssl-3-x64.dll /mt /z > nul
	ROBOCOPY HighLo\vendor\assimp\lib\Debug\ HighLoEdit\bin\Release-windows-x86_64\HighLoEdit\ assimp-vc142-mtd.dll /mt /z > nul
	ROBOCOPY HighLo\assets\ HighLoEdit\bin\Release-windows-x86_64\HighLoEdit\ editorconfig.ini /mt /z > nul
	ROBOCOPY HighLo\assets\ HighLoEdit\bin\Release-windows-x86_64\HighLoEdit\assets\ /MIR > nul
	ROBOCOPY HighLoEdit\assets\ HighLoEdit\bin\Release-windows-x86_64\HighLoEdit\assets\ /MIR > nul
	ECHO Copy done!
	ECHO.
)

if exist tests\bin\Debug-windows-x86_64\HighLoTest\ (
	ECHO copying into tests debug folder...
	ROBOCOPY HighLo\vendor\openssl\lib\ tests\bin\Debug-windows-x86_64\HighLoTest\ libcrypto-3-x64.dll /mt /z > nul
	ROBOCOPY HighLo\vendor\openssl\lib\ tests\bin\Debug-windows-x86_64\HighLoTest\ libssl-3-x64.dll /mt /z > nul
	ROBOCOPY HighLo\vendor\assimp\lib\Debug\ tests\bin\Debug-windows-x86_64\HighLoTest\ assimp-vc142-mtd.dll /mt /z > nul
	ECHO Copy done!
	ECHO.
)

if exist tests\bin\Release-windows-x86_64\HighLoTest\ (
	ECHO Copying into tests release folder...
	ROBOCOPY HighLo\vendor\openssl\lib\ tests\bin\Release-windows-x86_64\HighLoTest\ libcrypto-3-x64.dll /mt /z > nul
	ROBOCOPY HighLo\vendor\openssl\lib\ tests\bin\Release-windows-x86_64\HighLoTest\ libssl-3-x64.dll /mt /z > nul
	ROBOCOPY HighLo\vendor\assimp\lib\Debug\ tests\bin\Release-windows-x86_64\HighLoTest\ assimp-vc142-mtd.dll /mt /z > nul
	ECHO Copy done!
	ECHO.
)

popd
PAUSE