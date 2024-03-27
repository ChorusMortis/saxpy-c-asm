# saxpy-c-asm

Performance comparison of the SAXPY function in C and Assembly created for LBYARCH course.

## Installation

1. Clone the repository.
2. Open the project in Visual Studio 2022. Sample solution files (`.sln` and `.vcxproj`) for the settings needed to build the project are provided. Ensure they are placed in the root directory as Visual Studio 2022 expects the files to be found there.

    The configuration needed to build successfully are as follows. The settings for `saxpy.asm` are for linking it properly with the main C program. The `Enable Large Addresses` setting for the project itself is required to remove symbol relocation compilation errors in the Assembly code (see [LNK2017](https://learn.microsoft.com/en-us/cpp/error-messages/tool-errors/linker-tools-error-lnk2017)).

    | Project/File          | Property                    | Field                  | Value                      |
    | --------------------- | --------------------------- | ---------------------- | -------------------------- |
    | `saxpy.asm`           | General                     | Excluded From Build    | No                         |
    |                       |                             | Item Type              | Custom Build Tool          |
    |                       | Custom Build Tool > General | Command Line           | `nasm -f win64 saxpy.asm`  |
    |                       |                             | Outputs                | `saxpy.obj`                |
    | saxpy-c-asm (Project) | Linker > System             | Enable Large Addresses | No (/LARGEADDRESSAWARE:NO) |

3. Build the solution and run it.
