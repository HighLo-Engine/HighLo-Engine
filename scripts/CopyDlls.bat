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

if exist Sandbox\bin\Debug-windows-x86_64\Sandbox\ (
	ECHO copying into Sandbox debug folder...
	ROBOCOPY HighLo\vendor\openssl\lib\ Sandbox\bin\Debug-windows-x86_64\Sandbox\ libcrypto-3-x64.dll /mt /z > nul
	ROBOCOPY HighLo\vendor\openssl\lib\ Sandbox\bin\Debug-windows-x86_64\Sandbox\ libssl-3-x64.dll /mt /z > nul
	ROBOCOPY HighLo\vendor\assimp\lib\Debug\ Sandbox\bin\Debug-windows-x86_64\Sandbox\ assimp-vc142-mtd.dll /mt /z > nul
	ROBOCOPY HighLo\assets\ Sandbox\bin\Debug-windows-x86_64\Sandbox\ editorconfig.ini /mt /z > nul
	ROBOCOPY HighLo\assets\ Sandbox\bin\Debug-windows-x86_64\Sandbox\assets\ /MIR > nul
	ROBOCOPY Sandbox\assets\ Sandbox\bin\Debug-windows-x86_64\Sandbox\assets\ /MIR > nul
	ECHO Copy done!
	ECHO.
)

if exist Sandbox\bin\Release-windows-x86_64\Sandbox\ (
	ECHO Copying into Sandbox release folder...
	ROBOCOPY HighLo\vendor\openssl\lib\ Sandbox\bin\Release-windows-x86_64\Sandbox\ libcrypto-3-x64.dll /mt /z > nul
	ROBOCOPY HighLo\vendor\openssl\lib\ Sandbox\bin\Release-windows-x86_64\Sandbox\ libssl-3-x64.dll /mt /z > nul
	ROBOCOPY HighLo\vendor\assimp\lib\Debug\ Sandbox\bin\Release-windows-x86_64\Sandbox\ assimp-vc142-mtd.dll /mt /z > nul
	ROBOCOPY HighLo\assets\ Sandbox\bin\Release-windows-x86_64\Sandbox\ editorconfig.ini /mt /z > nul
	ROBOCOPY HighLo\assets\ Sandbox\bin\Release-windows-x86_64\Sandbox\assets\ /MIR > nul
	ROBOCOPY Sandbox\assets\ Sandbox\bin\Release-windows-x86_64\Sandbox\assets\ /MIR > nul
	ECHO Copy done!
	ECHO.
)

if exist demos\PBRSceneDemo\bin\Debug-windows-x86_64\PBRSceneDemo\ (
	ECHO copying into PBRSceneDemo debug folder...
	ROBOCOPY HighLo\vendor\openssl\lib\ demos\PBRSceneDemo\bin\Debug-windows-x86_64\PBRSceneDemo\ libcrypto-3-x64.dll /mt /z > nul
	ROBOCOPY HighLo\vendor\openssl\lib\ demos\PBRSceneDemo\bin\Debug-windows-x86_64\PBRSceneDemo\ libssl-3-x64.dll /mt /z > nul
	ROBOCOPY HighLo\vendor\assimp\lib\Debug\ demos\PBRSceneDemo\bin\Debug-windows-x86_64\PBRSceneDemo\ assimp-vc142-mtd.dll /mt /z > nul
	ROBOCOPY HighLo\assets\ demos\PBRSceneDemo\bin\Debug-windows-x86_64\PBRSceneDemo\ editorconfig.ini /mt /z > nul
	ROBOCOPY HighLo\assets\ demos\PBRSceneDemo\bin\Debug-windows-x86_64\PBRSceneDemo\assets\ /MIR > nul
	ROBOCOPY demos\PBRSceneDemo\assets\ demos\PBRSceneDemo\bin\Debug-windows-x86_64\PBRSceneDemo\assets\ /MIR > nul
	ECHO Copy done!
	ECHO.
)

if exist demos\PBRSceneDemo\bin\Release-windows-x86_64\PBRSceneDemo\ (
	ECHO Copying into PBRSceneDemo release folder...
	ROBOCOPY HighLo\vendor\openssl\lib\ demos\PBRSceneDemo\bin\Release-windows-x86_64\PBRSceneDemo\ libcrypto-3-x64.dll /mt /z > nul
	ROBOCOPY HighLo\vendor\openssl\lib\ demos\PBRSceneDemo\bin\Release-windows-x86_64\PBRSceneDemo\ libssl-3-x64.dll /mt /z > nul
	ROBOCOPY HighLo\vendor\assimp\lib\Debug\ demos\PBRSceneDemo\bin\Release-windows-x86_64\PBRSceneDemo\ assimp-vc142-mtd.dll /mt /z > nul
	ROBOCOPY HighLo\assets\ demos\PBRSceneDemo\bin\Release-windows-x86_64\PBRSceneDemo\ editorconfig.ini /mt /z > nul
	ROBOCOPY HighLo\assets\ demos\PBRSceneDemo\bin\Release-windows-x86_64\PBRSceneDemo\assets\ /MIR > nul
	ROBOCOPY demos\PBRSceneDemo\assets\ demos\PBRSceneDemo\bin\Release-windows-x86_64\PBRSceneDemo\assets\ /MIR > nul
	ECHO Copy done!
	ECHO.
)

if exist demos\SponzaSceneDemo\bin\Debug-windows-x86_64\SponzaSceneDemo\ (
	ECHO copying into SponzaSceneDemo debug folder...
	ROBOCOPY HighLo\vendor\openssl\lib\ demos\SponzaSceneDemo\bin\Debug-windows-x86_64\SponzaSceneDemo\ libcrypto-3-x64.dll /mt /z > nul
	ROBOCOPY HighLo\vendor\openssl\lib\ demos\SponzaSceneDemo\bin\Debug-windows-x86_64\SponzaSceneDemo\ libssl-3-x64.dll /mt /z > nul
	ROBOCOPY HighLo\vendor\assimp\lib\Debug\ demos\SponzaSceneDemo\bin\Debug-windows-x86_64\SponzaSceneDemo\ assimp-vc142-mtd.dll /mt /z > nul
	ROBOCOPY HighLo\assets\ demos\SponzaSceneDemo\bin\Debug-windows-x86_64\SponzaSceneDemo\ editorconfig.ini /mt /z > nul
	ROBOCOPY HighLo\assets\ demos\SponzaSceneDemo\bin\Debug-windows-x86_64\SponzaSceneDemo\assets\ /MIR > nul
	ROBOCOPY demos\SponzaSceneDemo\assets\ demos\SponzaSceneDemo\bin\Debug-windows-x86_64\SponzaSceneDemo\assets\ /MIR > nul
	ECHO Copy done!
	ECHO.
)

if exist demos\SponzaSceneDemo\bin\Release-windows-x86_64\SponzaSceneDemo\ (
	ECHO Copying into SponzaSceneDemo release folder...
	ROBOCOPY HighLo\vendor\openssl\lib\ demos\SponzaSceneDemo\bin\Release-windows-x86_64\SponzaSceneDemo\ libcrypto-3-x64.dll /mt /z > nul
	ROBOCOPY HighLo\vendor\openssl\lib\ demos\SponzaSceneDemo\bin\Release-windows-x86_64\SponzaSceneDemo\ libssl-3-x64.dll /mt /z > nul
	ROBOCOPY HighLo\vendor\assimp\lib\Debug\ demos\SponzaSceneDemo\bin\Release-windows-x86_64\SponzaSceneDemo\ assimp-vc142-mtd.dll /mt /z > nul
	ROBOCOPY HighLo\assets\ demos\SponzaSceneDemo\bin\Release-windows-x86_64\SponzaSceneDemo\ editorconfig.ini /mt /z > nul
	ROBOCOPY HighLo\assets\ demos\SponzaSceneDemo\bin\Release-windows-x86_64\SponzaSceneDemo\assets\ /MIR > nul
	ROBOCOPY demos\SponzaSceneDemo\assets\ demos\SponzaSceneDemo\bin\Release-windows-x86_64\SponzaSceneDemo\assets\ /MIR > nul
	ECHO Copy done!
	ECHO.
)

if exist demos\Renderer2DDemo\bin\Debug-windows-x86_64\Renderer2DDemo\ (
	ECHO copying into Renderer2DDemo debug folder...
	ROBOCOPY HighLo\vendor\openssl\lib\ demos\Renderer2DDemo\bin\Debug-windows-x86_64\Renderer2DDemo\ libcrypto-3-x64.dll /mt /z > nul
	ROBOCOPY HighLo\vendor\openssl\lib\ demos\Renderer2DDemo\bin\Debug-windows-x86_64\Renderer2DDemo\ libssl-3-x64.dll /mt /z > nul
	ROBOCOPY HighLo\vendor\assimp\lib\Debug\ demos\Renderer2DDemo\bin\Debug-windows-x86_64\Renderer2DDemo\ assimp-vc142-mtd.dll /mt /z > nul
	ROBOCOPY HighLo\assets\ demos\Renderer2DDemo\bin\Debug-windows-x86_64\Renderer2DDemo\ editorconfig.ini /mt /z > nul
	ROBOCOPY HighLo\assets\ demos\Renderer2DDemo\bin\Debug-windows-x86_64\Renderer2DDemo\assets\ /MIR > nul
	ROBOCOPY demos\Renderer2DDemo\assets\ demos\Renderer2DDemo\bin\Debug-windows-x86_64\Renderer2DDemo\assets\ /MIR > nul
	ECHO Copy done!
	ECHO.
)

if exist demos\Renderer2DDemo\bin\Release-windows-x86_64\Renderer2DDemo\ (
	ECHO Copying into Renderer2DDemo release folder...
	ROBOCOPY HighLo\vendor\openssl\lib\ demos\Renderer2DDemo\bin\Release-windows-x86_64\Renderer2DDemo\ libcrypto-3-x64.dll /mt /z > nul
	ROBOCOPY HighLo\vendor\openssl\lib\ demos\Renderer2DDemo\bin\Release-windows-x86_64\Renderer2DDemo\ libssl-3-x64.dll /mt /z > nul
	ROBOCOPY HighLo\vendor\assimp\lib\Debug\ demos\Renderer2DDemo\bin\Release-windows-x86_64\Renderer2DDemo\ assimp-vc142-mtd.dll /mt /z > nul
	ROBOCOPY HighLo\assets\ demos\Renderer2DDemo\bin\Release-windows-x86_64\Renderer2DDemo\ editorconfig.ini /mt /z > nul
	ROBOCOPY HighLo\assets\ demos\Renderer2DDemo\bin\Release-windows-x86_64\Renderer2DDemo\assets\ /MIR > nul
	ROBOCOPY demos\Renderer2DDemo\assets\ demos\Renderer2DDemo\bin\Release-windows-x86_64\Renderer2DDemo\assets\ /MIR > nul
	ECHO Copy done!
	ECHO.
)

popd
PAUSE

