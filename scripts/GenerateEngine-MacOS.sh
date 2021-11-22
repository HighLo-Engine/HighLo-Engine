#!/bin/bash

pushd ..
chmod +x vendor/bin/premake/MacOS/premake
vendor/bin/premake/MacOS/premake xcode4
popd

