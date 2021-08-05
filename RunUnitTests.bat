@echo off

echo preparing Unit Tests...

echo copying dlls...
ROBOCOPY HighLo\bin\Release-windows-x86_64\HighLo\ tests\StringTest\bin\ HighLo.dll /mt /z
ROBOCOPY HighLo\vendor\openssl\lib\ tests\StringTest\bin\ libcrypto-3-x64.dll /mt /z
ROBOCOPY HighLo\vendor\openssl\lib\ tests\StringTest\bin\ libssl-3-x64.dll /mt /z
ROBOCOPY HighLo\vendor\assimp\lib\Release\ tests\StringTest\bin\ assimp-vc142-mt.dll /mt /z
ROBOCOPY HighLo\vendor\HighLo-Unit\lib\ tests\StringTest\bin\ HighLo-Unit.dll /mt /z
echo done preparation.

echo Running String Unit Tests...
call tests\StringTest\bin\Release-windows-x86_64\StringTest\StringTest.exe || echo Some Unit tests have failed.

EXIT %ERRORLEVEL%

