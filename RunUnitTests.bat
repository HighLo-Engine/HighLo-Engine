@echo off

echo preparing Unit Tests...

echo copying dlls...
ROBOCOPY HighLo\bin\Release-windows-x86_64\HighLo\ tests\StringTest\bin\Release-windows-x86_64\StringTest\ HighLo.dll /mt /z
ROBOCOPY HighLo\vendor\openssl\lib\ tests\StringTest\bin\Release-windows-x86_64\StringTest\ libcrypto-3-x64.dll /mt /z
ROBOCOPY HighLo\vendor\openssl\lib\ tests\StringTest\bin\Release-windows-x86_64\StringTest\ libssl-3-x64.dll /mt /z
ROBOCOPY HighLo\vendor\assimp\lib\Release\ tests\StringTest\bin\Release-windows-x86_64\StringTest\ assimp-vc142-mt.dll /mt /z
echo done preperation.

echo Running String Unit Tests...
call tests\StringTest\bin\Release-windows-x86_64\StringTest\StringTest.exe

PAUSE