<p align="center">
  <img src="/HighLo.png" alt="HighLo-Logo" width="1024">
</p>

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
python scripts/GenerateEngine.py
```

# Building the engine

For detailed information, see the [Build guide](BUILDING.md).

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

# Gource statistics

Every time, when a pull request is merged into our master branch we use [gource](https://gource.io) to create this fancy looking contributing graph animation! You can see all users, who participated into this project and what they worked on.

<video src="https://www.highlo-engine.com/uploads/gource.mp4" width="640" autoplay muted loop></video>

