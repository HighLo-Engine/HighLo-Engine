@ECHO off

ECHO preparing Unit Tests...

ECHO copying dlls...
ROBOCOPY HighLo\bin\Release-windows-x86_64\HighLo\ tests\StringTest\bin\ HighLo.dll /mt /z > nul
ROBOCOPY HighLo\vendor\openssl\lib\ tests\StringTest\bin\ libcrypto-3-x64.dll /mt /z > nul
ROBOCOPY HighLo\vendor\openssl\lib\ tests\StringTest\bin\ libssl-3-x64.dll /mt /z > nul
ROBOCOPY HighLo\vendor\assimp\lib\Debug\ tests\StringTest\bin\ assimp-vc142-mtd.dll /mt /z > nul
ROBOCOPY HighLo\vendor\HighLo-Unit\lib\ tests\StringTest\bin\ HighLo-Unit.dll /mt /z > nul

ROBOCOPY HighLo\bin\Release-windows-x86_64\HighLo\ tests\EncryptionTest\bin\ HighLo.dll /mt /z > nul
ROBOCOPY HighLo\vendor\openssl\lib\ tests\EncryptionTest\bin\ libcrypto-3-x64.dll /mt /z > nul
ROBOCOPY HighLo\vendor\openssl\lib\ tests\EncryptionTest\bin\ libssl-3-x64.dll /mt /z > nul
ROBOCOPY HighLo\vendor\assimp\lib\Debug\ tests\EncryptionTest\bin\ assimp-vc142-mtd.dll /mt /z > nul
ROBOCOPY HighLo\vendor\HighLo-Unit\lib\ tests\EncryptionTest\bin\ HighLo-Unit.dll /mt /z > nul
ECHO OK: dlls have been copied!
ECHO.

ECHO Running String Unit Tests...
cd.>nul
CALL tests\StringTest\bin\StringTest.exe
IF %ERRORLEVEL% NEQ 0 EXIT /B %ERRORLEVEL%
ECHO.

ECHO Running Encryption Unit Tests...
cd.>nul
CALL tests\EncryptionTest\bin\EncryptionTest.exe
IF %ERRORLEVEL% NEQ 0 EXIT /B %ERRORLEVEL%
ECHO.

EXIT /B 0

