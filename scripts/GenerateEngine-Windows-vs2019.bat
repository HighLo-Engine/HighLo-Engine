@ECHO OFF

PUSHD ..
CALL vendor\bin\premake\Windows\premake5.exe vs2019
POPD

