@ECHO off

pushd ..

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

ROBOCOPY HighLo\vendor\openssl\lib\ tests\ECSTest\bin\ libcrypto-3-x64.dll /mt /z > nul
ROBOCOPY HighLo\vendor\openssl\lib\ tests\ECSTest\bin\ libssl-3-x64.dll /mt /z > nul
ROBOCOPY HighLo\vendor\assimp\lib\Debug\ tests\ECSTest\bin\ assimp-vc142-mtd.dll /mt /z > nul
ROBOCOPY HighLo\vendor\HighLo-Unit\lib\ tests\ECSTest\bin\ HighLo-Unit.dll /mt /z > nul

ROBOCOPY HighLo\vendor\openssl\lib\ tests\FileSystemTest\bin\ libcrypto-3-x64.dll /mt /z > nul
ROBOCOPY HighLo\vendor\openssl\lib\ tests\FileSystemTest\bin\ libssl-3-x64.dll /mt /z > nul
ROBOCOPY HighLo\vendor\assimp\lib\Debug\ tests\FileSystemTest\bin\ assimp-vc142-mtd.dll /mt /z > nul
ROBOCOPY HighLo\vendor\HighLo-Unit\lib\ tests\FileSystemTest\bin\ HighLo-Unit.dll /mt /z > nul

ROBOCOPY HighLo\vendor\openssl\lib\ tests\FileSystemPathTest\bin\ libcrypto-3-x64.dll /mt /z > nul
ROBOCOPY HighLo\vendor\openssl\lib\ tests\FileSystemPathTest\bin\ libssl-3-x64.dll /mt /z > nul
ROBOCOPY HighLo\vendor\assimp\lib\Debug\ tests\FileSystemPathTest\bin\ assimp-vc142-mtd.dll /mt /z > nul
ROBOCOPY HighLo\vendor\HighLo-Unit\lib\ tests\FileSystemPathTest\bin\ HighLo-Unit.dll /mt /z > nul

ROBOCOPY HighLo\vendor\openssl\lib\ tests\FileTest\bin\ libcrypto-3-x64.dll /mt /z > nul
ROBOCOPY HighLo\vendor\openssl\lib\ tests\FileTest\bin\ libssl-3-x64.dll /mt /z > nul
ROBOCOPY HighLo\vendor\assimp\lib\Debug\ tests\FileTest\bin\ assimp-vc142-mtd.dll /mt /z > nul
ROBOCOPY HighLo\vendor\HighLo-Unit\lib\ tests\FileTest\bin\ HighLo-Unit.dll /mt /z > nul

ROBOCOPY HighLo\vendor\openssl\lib\ tests\BinaryTreeTest\bin\ libcrypto-3-x64.dll /mt /z > nul
ROBOCOPY HighLo\vendor\openssl\lib\ tests\BinaryTreeTest\bin\ libssl-3-x64.dll /mt /z > nul
ROBOCOPY HighLo\vendor\assimp\lib\Debug\ tests\BinaryTreeTest\bin\ assimp-vc142-mtd.dll /mt /z > nul
ROBOCOPY HighLo\vendor\HighLo-Unit\lib\ tests\BinaryTreeTest\bin\ HighLo-Unit.dll /mt /z > nul

ROBOCOPY HighLo\vendor\openssl\lib\ tests\BinarySearchTreeTest\bin\ libcrypto-3-x64.dll /mt /z > nul
ROBOCOPY HighLo\vendor\openssl\lib\ tests\BinarySearchTreeTest\bin\ libssl-3-x64.dll /mt /z > nul
ROBOCOPY HighLo\vendor\assimp\lib\Debug\ tests\BinarySearchTreeTest\bin\ assimp-vc142-mtd.dll /mt /z > nul
ROBOCOPY HighLo\vendor\HighLo-Unit\lib\ tests\BinarySearchTreeTest\bin\ HighLo-Unit.dll /mt /z > nul

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

ECHO Running ECS Unit Tests...
cd.>nul
CALL tests\ECSTest\bin\ECSTest.exe
IF %ERRORLEVEL% NEQ 0 EXIT /B %ERRORLEVEL%
ECHO.

ECHO Running FileSystem Unit Tests...
cd.>nul
CALL tests\FileSystemTest\bin\FileSystemTest.exe
IF %ERRORLEVEL% NEQ 0 EXIT /B %ERRORLEVEL%
ECHO.

ECHO Running File Unit Tests...
cd.>nul
CALL tests\FileTest\bin\FileTest.exe
IF %ERRORLEVEL% NEQ 0 EXIT /B %ERRORLEVEL%
ECHO.

ECHO Running FileSystemPath Unit Tests...
cd.>nul
CALL tests\FileSystemPathTest\bin\FileSystemPathTest.exe
IF %ERRORLEVEL% NEQ 0 EXIT /B %ERRORLEVEL%
ECHO.

ECHO Running BinaryTree Unit Tests...
cd.>nul
CALL tests\BinaryTreeTest\bin\BinaryTreeTest.exe
IF %ERRORLEVEL% NEQ 0 EXIT /B %ERRORLEVEL%
ECHO.

ECHO Running BinarySearchTree Unit Tests...
cd.>nul
CALL tests\BinarySearchTreeTest\bin\BinarySearchTreeTest.exe
IF %ERRORLEVEL% NEQ 0 EXIT /B %ERRORLEVEL%
ECHO.

popd

EXIT /B 0

