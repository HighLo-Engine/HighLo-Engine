@ECHO off

pushd ..

if exist HighLoEdit\bin\Debug-OpenGL-windows-x86_64\HighLoEdit\ (
	ECHO copying into HighLoEdit debug folder...
	ROBOCOPY HighLo\vendor\openssl\lib\ HighLoEdit\bin\Debug-OpenGL-windows-x86_64\HighLoEdit\ libcrypto-3-x64.dll /mt /z > nul
	ROBOCOPY HighLo\vendor\openssl\lib\ HighLoEdit\bin\Debug-OpenGL-windows-x86_64\HighLoEdit\ libssl-3-x64.dll /mt /z > nul
	ROBOCOPY HighLo\vendor\assimp\lib\Debug\ HighLoEdit\bin\Debug-OpenGL-windows-x86_64\HighLoEdit\ assimp-vc142-mtd.dll /mt /z > nul
	ROBOCOPY HighLo\assets\ HighLoEdit\bin\Debug-OpenGL-windows-x86_64\HighLoEdit\ editorconfig.ini /mt /z > nul
	ROBOCOPY HighLo\assets\ HighLoEdit\bin\Debug-OpenGL-windows-x86_64\HighLoEdit\assets\ /MIR > nul
	ROBOCOPY HighLoEdit\assets\ HighLoEdit\bin\Debug-OpenGL-windows-x86_64\HighLoEdit\assets\ /MIR > nul
	ECHO Copy done!
	ECHO.
)

if exist HighLoEdit\bin\Release-OpenGL-windows-x86_64\HighLoEdit\ (
	ECHO Copying into HighLoEdit release folder...
	ROBOCOPY HighLo\vendor\openssl\lib\ HighLoEdit\bin\Release-OpenGL-windows-x86_64\HighLoEdit\ libcrypto-3-x64.dll /mt /z > nul
	ROBOCOPY HighLo\vendor\openssl\lib\ HighLoEdit\bin\Release-OpenGL-windows-x86_64\HighLoEdit\ libssl-3-x64.dll /mt /z > nul
	ROBOCOPY HighLo\vendor\assimp\lib\Debug\ HighLoEdit\bin\Release-OpenGL-windows-x86_64\HighLoEdit\ assimp-vc142-mtd.dll /mt /z > nul
	ROBOCOPY HighLo\assets\ HighLoEdit\bin\Release-OpenGL-windows-x86_64\HighLoEdit\ editorconfig.ini /mt /z > nul
	ROBOCOPY HighLo\assets\ HighLoEdit\bin\Release-OpenGL-windows-x86_64\HighLoEdit\assets\ /MIR > nul
	ROBOCOPY HighLoEdit\assets\ HighLoEdit\bin\Release-OpenGL-windows-x86_64\HighLoEdit\assets\ /MIR > nul
	ECHO Copy done!
	ECHO.
)

if exist tests\bin\Debug-OpenGL-windows-x86_64\HighLoTest\ (
	ECHO copying into tests debug folder...
	ROBOCOPY HighLo\vendor\openssl\lib\ tests\bin\Debug-OpenGL-windows-x86_64\HighLoTest\ libcrypto-3-x64.dll /mt /z > nul
	ROBOCOPY HighLo\vendor\openssl\lib\ tests\bin\Debug-OpenGL-windows-x86_64\HighLoTest\ libssl-3-x64.dll /mt /z > nul
	ROBOCOPY HighLo\vendor\assimp\lib\Debug\ tests\bin\Debug-OpenGL-windows-x86_64\HighLoTest\ assimp-vc142-mtd.dll /mt /z > nul
	ECHO Copy done!
	ECHO.
)

if exist tests\bin\Release-OpenGL-windows-x86_64\HighLoTest\ (
	ECHO Copying into tests release folder...
	ROBOCOPY HighLo\vendor\openssl\lib\ tests\bin\Release-OpenGL-windows-x86_64\HighLoTest\ libcrypto-3-x64.dll /mt /z > nul
	ROBOCOPY HighLo\vendor\openssl\lib\ tests\bin\Release-OpenGL-windows-x86_64\HighLoTest\ libssl-3-x64.dll /mt /z > nul
	ROBOCOPY HighLo\vendor\assimp\lib\Debug\ tests\bin\Release-OpenGL-windows-x86_64\HighLoTest\ assimp-vc142-mtd.dll /mt /z > nul
	ECHO Copy done!
	ECHO.
)

if exist Sandbox\bin\Debug-OpenGL-windows-x86_64\Sandbox\ (
	ECHO copying into Sandbox debug folder...
	ROBOCOPY HighLo\vendor\openssl\lib\ Sandbox\bin\Debug-OpenGL-windows-x86_64\Sandbox\ libcrypto-3-x64.dll /mt /z > nul
	ROBOCOPY HighLo\vendor\openssl\lib\ Sandbox\bin\Debug-OpenGL-windows-x86_64\Sandbox\ libssl-3-x64.dll /mt /z > nul
	ROBOCOPY HighLo\vendor\assimp\lib\Debug\ Sandbox\bin\Debug-OpenGL-windows-x86_64\Sandbox\ assimp-vc142-mtd.dll /mt /z > nul
	ROBOCOPY HighLo\assets\ Sandbox\bin\Debug-OpenGL-windows-x86_64\Sandbox\ editorconfig.ini /mt /z > nul
	ROBOCOPY HighLo\assets\ Sandbox\bin\Debug-OpenGL-windows-x86_64\Sandbox\assets\ /MIR > nul
	ROBOCOPY Sandbox\assets\ Sandbox\bin\Debug-OpenGL-windows-x86_64\Sandbox\assets\ /MIR > nul
	ECHO Copy done!
	ECHO.
)

if exist Sandbox\bin\Release-OpenGL-windows-x86_64\Sandbox\ (
	ECHO Copying into Sandbox release folder...
	ROBOCOPY HighLo\vendor\openssl\lib\ Sandbox\bin\Release-OpenGL-windows-x86_64\Sandbox\ libcrypto-3-x64.dll /mt /z > nul
	ROBOCOPY HighLo\vendor\openssl\lib\ Sandbox\bin\Release-OpenGL-windows-x86_64\Sandbox\ libssl-3-x64.dll /mt /z > nul
	ROBOCOPY HighLo\vendor\assimp\lib\Debug\ Sandbox\bin\Release-OpenGL-windows-x86_64\Sandbox\ assimp-vc142-mtd.dll /mt /z > nul
	ROBOCOPY HighLo\assets\ Sandbox\bin\Release-OpenGL-windows-x86_64\Sandbox\ editorconfig.ini /mt /z > nul
	ROBOCOPY HighLo\assets\ Sandbox\bin\Release-OpenGL-windows-x86_64\Sandbox\assets\ /MIR > nul
	ROBOCOPY Sandbox\assets\ Sandbox\bin\Release-OpenGL-windows-x86_64\Sandbox\assets\ /MIR > nul
	ECHO Copy done!
	ECHO.
)

popd
PAUSE

