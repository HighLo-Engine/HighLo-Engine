@ECHO off

pushd ..

if exist HighLoEdit\bin\Debug-windows-x86_64\HighLoEdit\ (
	ECHO copying into debug folder...
	ROBOCOPY HighLo\vendor\openssl\lib\ HighLoEdit\bin\Debug-windows-x86_64\HighLoEdit\ libcrypto-3-x64.dll /mt /z > nul
	ROBOCOPY HighLo\vendor\openssl\lib\ HighLoEdit\bin\Debug-windows-x86_64\HighLoEdit\ libssl-3-x64.dll /mt /z > nul
	ROBOCOPY HighLo\vendor\assimp\lib\Debug\ HighLoEdit\bin\Debug-windows-x86_64\HighLoEdit\ assimp-vc142-mtd.dll /mt /z > nul
	ROBOCOPY HighLo\vendor\HighLo-Unit\lib\ HighLoEdit\bin\Debug-windows-x86_64\HighLoEdit\ HighLo-Unit.dll /mt /z > nul
	ROBOCOPY HighLo\assets\ HighLoEdit\bin\Debug-windows-x86_64\HighLoEdit\ editorconfig.ini /mt /z > nul
	ROBOCOPY HighLo\assets\ HighLoEdit\bin\Debug-windows-x86_64\HighLoEdit\assets\ /MIR > nul
	ROBOCOPY HighLoEdit\assets\ HighLoEdit\bin\Debug-windows-x86_64\HighLoEdit\assets\ /MIR > nul
	ECHO Copy done!
)

if exist HighLoEdit\bin\Release-windows-x86_64\HighLoEdit\ (
	ECHO Copying into release folder...
	ROBOCOPY HighLo\vendor\openssl\lib\ HighLoEdit\bin\Release-windows-x86_64\HighLoEdit\ libcrypto-3-x64.dll /mt /z > nul
	ROBOCOPY HighLo\vendor\openssl\lib\ HighLoEdit\bin\Release-windows-x86_64\HighLoEdit\ libssl-3-x64.dll /mt /z > nul
	ROBOCOPY HighLo\vendor\assimp\lib\Debug\ HighLoEdit\bin\Release-windows-x86_64\HighLoEdit\ assimp-vc142-mtd.dll /mt /z > nul
	ROBOCOPY HighLo\vendor\HighLo-Unit\lib\ HighLoEdit\bin\Release-windows-x86_64\HighLoEdit\ HighLo-Unit.dll /mt /z > nul
	ROBOCOPY HighLo\assets\ HighLoEdit\bin\Release-windows-x86_64\HighLoEdit\ editorconfig.ini /mt /z > nul
	ROBOCOPY HighLo\assets\ HighLoEdit\bin\Release-windows-x86_64\HighLoEdit\assets\ /MIR > nul
	ROBOCOPY HighLoEdit\assets\ HighLoEdit\bin\Release-windows-x86_64\HighLoEdit\assets\ /MIR > nul
	ECHO Copy done!
)

popd
PAUSE