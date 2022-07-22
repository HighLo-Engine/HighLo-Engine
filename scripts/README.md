# The build system

Our build system uses premake5 and python to generate the visual studio files and setup all build dependencies automatically. You can either select the `GenerateEngine.bat` file to use all default settings or you can call the `GenerateEngine.py` directly with providing your desired command line arguments.

## Basic usage

```sh
# This builds the visual studio files with default settings (no unit tests and visual studio version 2022)
python GenerateEngine.py
```

## Generate older visual studio versions

```sh
python GenerateEngine.py --visual-studio 2019
```

## Generate Unit tests

```sh
python GenerateEngine.py --generate-unit-tests
```

## Options

| Name        | Optional | Default | Description     |
| :---        |  :----:  | :----: |         :--- |
| --version              |   False  |  None  | Show the version of the build system and quit
| --help                 |   False  |  None  | Show the help menu and quit
| --visual-studio        |   False  |  2022  | Set the desired visual studio version to generate (either 2022 or 2019)
| --generate-unit-tests  |   False  |  False  | Enable the Unit tests (they will be included into visual studio as another project in the workspace.)

