@ECHO off

ECHO preparing Unit Tests...

ECHO copying dlls...
ROBOCOPY HighLo\bin\Release-windows-x86_64\HighLo\ tests\StringTest\bin\ HighLo.dll /mt /z
ROBOCOPY HighLo\vendor\openssl\lib\ tests\StringTest\bin\ libcrypto-3-x64.dll /mt /z
ROBOCOPY HighLo\vendor\openssl\lib\ tests\StringTest\bin\ libssl-3-x64.dll /mt /z
ROBOCOPY HighLo\vendor\assimp\lib\Debug\ tests\StringTest\bin\ assimp-vc142-mtd.dll /mt /z
ROBOCOPY HighLo\vendor\HighLo-Unit\lib\ tests\StringTest\bin\ HighLo-Unit.dll /mt /z
ECHO done preparation.

ECHO Running String Unit Tests...
cd.>nul
ECHO BEFORE: %ERRORLEVEL%
call tests\StringTest\bin\StringTest.exe
ECHO AFTER: %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 0 EXIT /B %ERRORLEVEL%

ECHO All Unit Tests have passed successfully
EXIT /B 0

