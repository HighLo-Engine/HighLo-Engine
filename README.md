<p align="center">
  <img src="/HighLo.png" alt="HighLo-Logo" width="1024">
</p>

# HighLo-Engine

The HighLo Engine is a free open source game engine made only by two very passionate programmers.

It's primary purpose is to learn about 3D graphics and game engine design in general. But in the last year we developed ideas of features that we would like to implement as well, which will be the core-features of this engine at some point. Further information will follow, as soon as we built up the base engine :)

Feel free to contribute at any time, but it would be better to discuss major changes in an issue first.

# Getting started

```sh
$ git clone --recursive https://github.com/HighLo-Engine/HighLo-Engine.git/
```

```sh
$ cd HighLo-Engine
```

```sh
# Generate the Solution files for windows and mac or the makefiles for linux
$ python scripts/GenerateEngine.py
```

# Run Unit Tests

To Run the unit tests, you have to build the project from source.
After that you will be able to select the "HighLoTest" project as the starting project and execute it.

# Troubleshooting
1. `GenerateEngine.py` fails because the `requests` module is missing: `pip install requests`
2. `GenerateEngine.py` fails because the `fake_useragent` module is missing: `pip install fake_useragent`

These two modules are needed to automatically download the right Vulkan SDK from the official website. Normally they should be installed automatically if they are detected to be missing, but in some cases it might not work and the script will crash. This might have something to do with administrator privileges, so before installing the modules manually you also could try to run the script as an administrator.

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

