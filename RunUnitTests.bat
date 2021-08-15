@ECHO off

ECHO preparing Unit Tests...

ECHO copying dlls...
ROBOCOPY HighLo\bin\Release-windows-x86_64\HighLo\ tests\StringTest\bin\ HighLo.dll /mt /z > nul
ROBOCOPY HighLo\vendor\openssl\lib\ tests\StringTest\bin\ libcrypto-3-x64.dll /mt /z > nul
ROBOCOPY HighLo\vendor\openssl\lib\ tests\StringTest\bin\ libssl-3-x64.dll /mt /z > nul
ROBOCOPY HighLo\vendor\assimp\lib\Debug\ tests\StringTest\bin\ assimp-vc142-mtd.dll /mt /z > nul
ROBOCOPY HighLo\vendor\HighLo-Unit\lib\ tests\StringTest\bin\ HighLo-Unit.dll /mt /z > nul
ECHO OK: dlls have been copied!

ECHO Running String Unit Tests...

cd.>nul
CALL tests\StringTest\bin\StringTest.exe
IF %ERRORLEVEL% NEQ 0 EXIT /B %ERRORLEVEL%

EXIT /B 0

