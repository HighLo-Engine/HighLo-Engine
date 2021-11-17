#!/bin/bash

pushd ..
chmod +x ./vendor/bin/premake/Linux/premake5
./vendor/bin/premake/Linux/premake5 gmake
popd

