#!/bin/bash

pushd ..
chmod +x ./vendor/bin/premake/MacOS/premake5
./vendor/bin/premake/MacOS/premake5 xcode4
popd

