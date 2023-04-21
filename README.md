<p align="center">
  <img src="/HighLo.png" alt="HighLo-Logo" width="1024">
</p>

[![TODOs](https://badgen.net/https/api.tickgit.com/badgen/github.com/HighLo-Engine/HighLo-Engine)](https://www.tickgit.com/browse?repo=github.com/HighLo-Engine/HighLo-Engine)

# HighLo-Engine

The HighLo Engine is a free open source game engine made only by two very passionate programmers.

Its primary purpose is to learn about 3D graphics and game engine design in general. But in the last year we developed ideas of features that we would like to implement as well, which will be the core-features of this engine at some point. Further information will follow, as soon as we developed the base engine :)

Feel free to contribute at any time, but it would be better to discuss major changes in an issue first.

# Getting started

```sh
git clone --recursive https://github.com/HighLo-Engine/HighLo-Engine.git/
```

```sh
cd HighLo-Engine
```

```sh
# generate the solution files for mac or the makefiles for linux or the visual studio solution files for windows
python Scripts/GenerateEngine.py
```

# Building the engine

For detailed information, see the [Build guide](BUILDING.md).

# Building the working demo

Currently, the demo project is now working anymore due to heavy architecture and feature changes.
But you can build the demo application, by building an older version of the engine.
To build the demo application and run the state of the screenshots, follow these instructions:

```sh
# Clone the repository without submodules
git clone https://github.com/HighLo-Engine/HighLo-Engine.git
```

```sh
# Then checkout the commit, that contains the working demo version (f183fcf)
git checkout -q f183fcf
```

```sh
# Now get all submodules
git submodules update --init --recursive
```

```sh
# Execute the setup script, you can just double-click the file
./scripts/GenerateEngine.bat
```

Now you should be able to open the visual studio solution and build and run the demo application.
The demo application is located in `Demos/PBRSceneDemo` and you have to select this project as the **startup project** inside visual studio.

# Supported platforms

| Windows | Linux | MacOS | Android | IOS | Web |
|:--------:| :-: | :-: | :-: | :-: | :-: |
| ✓    | X | X | X | X | X |

# Supported rendering APIs

| OpenGL | Vulkan | DirectX 11 | DirectX 12 | Metal |
|:--------:| :-: | :-: | :-: | :-: |
| ✓    | X | X | X | X |

# Contributing

You can always create pull requests but keep in mind that the engine is in active development right now.

# Our working method

We use the feature-branch working method. This means that new features are being implemented in their own branches. If the development of the feature is finished, the branch gets merged into the master branch.

When the engine gets released, version branches will be created to maintain single versions. The master branch will always contain the latest code.

# Screenshots

![HighLo-Engine](/Screenshots/HighLo-Screenshot-1.png?raw=true "HighLo-Engine-1")
![HighLo-Engine](/Screenshots/HighLo-Screenshot-2.png?raw=true "HighLo-Engine-2")
![HighLo-Engine](/Screenshots/HighLo-Screenshot-3.png?raw=true "HighLo-Engine-3")
![HighLo-Engine](/Screenshots/HighLo-Screenshot-4.png?raw=true "HighLo-Engine-4")

