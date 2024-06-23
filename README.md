# Connect4 Bot

This is a project I made to help me learn C++. It's a relatively simple project that uses CMake as a build system.

### Building

To build the project, follow these steps:

1. Navigate to the project directory in your terminal.
2. Run the following command to generate the build files:

```bash
cmake -S . -B build
```
This will create a `build` directory in your project folder where all the build files are stored.

3. After the build files have been generated, you can build the project with:

```bash
cmake --build build
```

4. When the project is build, you can run it:

```bash
./build/Connect4
```