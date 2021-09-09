@ECHO off

ECHO preparing Unit Tests...

ECHO copying dlls...
ROBOCOPY HighLo\vendor\openssl\lib\ tests\StringTest\bin\ libcrypto-3-x64.dll /mt /z > nul
ROBOCOPY HighLo\vendor\openssl\lib\ tests\StringTest\bin\ libssl-3-x64.dll /mt /z > nul
ROBOCOPY HighLo\vendor\assimp\lib\Debug\ tests\StringTest\bin\ assimp-vc142-mtd.dll /mt /z > nul
ROBOCOPY HighLo\vendor\HighLo-Unit\lib\ tests\StringTest\bin\ HighLo-Unit.dll /mt /z > nul

ROBOCOPY HighLo\vendor\openssl\lib\ tests\EncryptionTest\bin\ libcrypto-3-x64.dll /mt /z > nul
ROBOCOPY HighLo\vendor\openssl\lib\ tests\EncryptionTest\bin\ libssl-3-x64.dll /mt /z > nul
ROBOCOPY HighLo\vendor\assimp\lib\Debug\ tests\EncryptionTest\bin\ assimp-vc142-mtd.dll /mt /z > nul
ROBOCOPY HighLo\vendor\HighLo-Unit\lib\ tests\EncryptionTest\bin\ HighLo-Unit.dll /mt /z > nul

ROBOCOPY HighLo\vendor\openssl\lib\ tests\ListTest\bin\ libcrypto-3-x64.dll /mt /z > nul
ROBOCOPY HighLo\vendor\openssl\lib\ tests\ListTest\bin\ libssl-3-x64.dll /mt /z > nul
ROBOCOPY HighLo\vendor\assimp\lib\Debug\ tests\ListTest\bin\ assimp-vc142-mtd.dll /mt /z > nul
ROBOCOPY HighLo\vendor\HighLo-Unit\lib\ tests\ListTest\bin\ HighLo-Unit.dll /mt /z > nul

ROBOCOPY HighLo\vendor\openssl\lib\ tests\HashmapTest\bin\ libcrypto-3-x64.dll /mt /z > nul
ROBOCOPY HighLo\vendor\openssl\lib\ tests\HashmapTest\bin\ libssl-3-x64.dll /mt /z > nul
ROBOCOPY HighLo\vendor\assimp\lib\Debug\ tests\HashmapTest\bin\ assimp-vc142-mtd.dll /mt /z > nul
ROBOCOPY HighLo\vendor\HighLo-Unit\lib\ tests\HashmapTest\bin\ HighLo-Unit.dll /mt /z > nul

ROBOCOPY HighLo\vendor\openssl\lib\ tests\VectorTest\bin\ libcrypto-3-x64.dll /mt /z > nul
ROBOCOPY HighLo\vendor\openssl\lib\ tests\VectorTest\bin\ libssl-3-x64.dll /mt /z > nul
ROBOCOPY HighLo\vendor\assimp\lib\Debug\ tests\VectorTest\bin\ assimp-vc142-mtd.dll /mt /z > nul
ROBOCOPY HighLo\vendor\HighLo-Unit\lib\ tests\VectorTest\bin\ HighLo-Unit.dll /mt /z > nul

ROBOCOPY HighLo\vendor\openssl\lib\ tests\QueueTest\bin\ libcrypto-3-x64.dll /mt /z > nul
ROBOCOPY HighLo\vendor\openssl\lib\ tests\QueueTest\bin\ libssl-3-x64.dll /mt /z > nul
ROBOCOPY HighLo\vendor\assimp\lib\Debug\ tests\QueueTest\bin\ assimp-vc142-mtd.dll /mt /z > nul
ROBOCOPY HighLo\vendor\HighLo-Unit\lib\ tests\QueueTest\bin\ HighLo-Unit.dll /mt /z > nul

ROBOCOPY HighLo\vendor\openssl\lib\ tests\StackTest\bin\ libcrypto-3-x64.dll /mt /z > nul
ROBOCOPY HighLo\vendor\openssl\lib\ tests\StackTest\bin\ libssl-3-x64.dll /mt /z > nul
ROBOCOPY HighLo\vendor\assimp\lib\Debug\ tests\StackTest\bin\ assimp-vc142-mtd.dll /mt /z > nul
ROBOCOPY HighLo\vendor\HighLo-Unit\lib\ tests\StackTest\bin\ HighLo-Unit.dll /mt /z > nul



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

ECHO Running List Unit Tests...
cd.>nul
CALL tests\ListTest\bin\ListTest.exe
IF %ERRORLEVEL% NEQ 0 EXIT /B %ERRORLEVEL%
ECHO.

ECHO Running Hashmap Unit Tests...
cd.>nul
CALL tests\HashmapTest\bin\HashmapTest.exe
IF %ERRORLEVEL% NEQ 0 EXIT /B %ERRORLEVEL%
ECHO.

ECHO Running Vector Unit Tests...
cd.>nul
CALL tests\VectorTest\bin\VectorTest.exe
IF %ERRORLEVEL% NEQ 0 EXIT /B %ERRORLEVEL%
ECHO.

ECHO Running Queue Unit Tests...
cd.>nul
CALL tests\QueueTest\bin\QueueTest.exe
IF %ERRORLEVEL% NEQ 0 EXIT /B %ERRORLEVEL%
ECHO.

ECHO Running Stack Unit Tests...
cd.>nul
CALL tests\StackTest\bin\StackTest.exe
IF %ERRORLEVEL% NEQ 0 EXIT /B %ERRORLEVEL%
ECHO.


PAUSE
EXIT /B 0

