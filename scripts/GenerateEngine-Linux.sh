#!/bin/bash

pushd ..
chmod +x vendor/bin/premake/Linux/premake
vendor/bin/premake/Linux/premake gmake
popd

