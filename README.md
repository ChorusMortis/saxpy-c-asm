# saxpy-c-asm

Performance comparison of the SAXPY function in C and Assembly created for LBYARCH course.

## Installation

1. Clone the repository.
2. Open the solution file (`.sln`) in Visual Studio 2022. Sample project files (`.vcxproj`) containing the settings needed to build the project are provided. Ensure they are placed in the root directory, as Visual Studio 2022 expects the files to be found there. The complete list of these configurations can be found in the table below.
    - The settings for `saxpy.asm` are for linking it properly with the main C program.
    - The command used for the custom build tool assumes that [NASM](https://nasm.us/) is installed and added to [PATH](https://en.wikipedia.org/wiki/PATH_(variable)), but an absolute file path to the NASM assembler can be used as well (i.e. `path/to/nasm.exe` instead of `nasm`).
    - Adding `legacy_stdio_definitions.lib` as an additional dependency for the linker can help avoid linking issues as the Assembly code calls the C function `printf`.

    | File/Project          | Property                    | Field                   | Value                                                    |
    | --------------------- | --------------------------- | ----------------------- | -------------------------------------------------------- |
    | `saxpy.asm`           | General                     | Excluded From Build     | No                                                       |
    |                       |                             | Item Type               | Custom Build Tool                                        |
    |                       | Custom Build Tool > General | Command Line            | `nasm -f win64 saxpy.asm`                                |
    |                       |                             | Outputs                 | `saxpy.obj`                                              |
    | saxpy-c-asm (Project) | Linker > Input              | Additional Dependencies | `legacy_stdio_definitions.lib;%(AdditionalDependencies)` |

3. Set the desired configuration in building the project (i.e., either Debug or Release mode).
4. Build the solution and run it.

## Performance Analysis

The following table shows the average execution times in milliseconds (rounded up to 5 decimal places) of the C and Assembly kernels over 30 runs each in debug and release mode for the different vector length $n$ values: $2^{20}$, $2^{24}$, and $2^{30}$. The complete list of execution times of all runs per value of $n$ in both debug and release modes can be found in the [execution times sheets](https://github.com/ChorusMortis/saxpy-c-asm/blob/main/docs/sheets_execution-times-per-value-of-n.pdf) located in the docs folder of this repository.

| Vector length (n)  | $2^{20}$ | $2^{24}$ | $2^{30}$    |
| ------------------ | -------- | -------- | ----------- |
| C (Debug)          | 3.66667  | 44.93333 | 59999.73333 |
| C (Release)        | 2.13333  | 31.16667 | 53279.73333 |
| Assembly (Debug)   | 3.8      | 47.83333 | 5253.7      |
| Assembly (Release) | 3.46667  | 55.9     | 3014.53333  |

In terms of the results for the execution times using the C language, the table shows that in the smallest value of $n = 2^{20}$, the release mode for C at $2.13333$ ms had a slightly faster execution time than the debug mode at $3.66667$ ms, with a slight difference of $1.53333$ ms. Having an average time of $31.16667$ ms in release mode and $44.93333$ ms in debug mode for $n = 2^{24}$, the C language's difference between the two modes grew into a significant $13.76667$ ms, highlighting the effect of the growth of the number of elements for each vector. Meanwhile, in the largest value of $n = 2^{30}$, the execution times grew exponentially at $59999.73333$ ms for the debug mode and $53279.73333$ ms for the release mode, which converts into an almost one-minute running time for each mode. The gap between the two modes also increased massively at $6720$ ms, which converts into more than six seconds. Observing the execution times for both debug and release modes for C, we can conclude that the compiler optimizations used for the release mode can significantly improve the performance of the C kernel as compared to the debug mode.

On the other hand, the results for the execution times using the Assembly language show a slightly different behavior than the C language. For $n = 2^{20}$, the execution time of the Assembly kernel in release mode was marginally faster at $3.46667$ ms compared to the $3.8$ ms in debug mode, with a slight disparity of $0.33333$ ms. Meanwhile, for $n = 2^{24}$, the release mode seems to have worsened the execution time of the Assembly kernel at $55.9$ ms compared to the slightly faster $47.83333$ ms of the debug mode with a difference of $8.06667$ ms. However, the most considerable value of $n = 2^{30}$ proved significant for the release mode as the Assembly kernel executed at $3014.53333$ ms only, while the debug mode ran at $5253.7$ ms, which had a $2239.16667$ ms difference. In general, the compiler optimizations for the release mode can also improve the execution times of the Assembly kernel. However, it may not be the same for all instances and can vary depending on the specifications of the machine used. Furthermore, the improvement may also be more significantly apparent for larger values of $n$ only since the lower values can produce inconsistencies.

Moreover, the table indicates that in debug mode for $n = 2^{20}$, the C kernel's execution time of 2.96667 ms is slower than the Assembly kernel's time which clocks in at 0.9 ms, a modest discrepancy of 2.06667 ms. As the input size increases to $n = 2^{24}$, the C kernel's time of 41.43333 ms becomes even slower compared to the Assembly kernel's time of 10.53333 ms, with a much more pronounced gap of 30.9 ms. For $n = 2^{28}$, the C kernel's execution time of 675.53333 ms in contrast with the Assembly kernel's execution time of 176.13333 ms creates a substantial time difference of 499.4 ms. At $n = 2^{30}$, the C kernel's runtime of 60136.4 ms or 60.1364 seconds compared to the Assembly kernel's runtime of 5361.36667 ms or 5.36137 seconds results in a gigantic time difference of 54775.03333 ms or 54.77503 seconds. This shows that in debug mode, Assembly code can be significantly faster compared to C code.

However, the table further reveals that in release mode, for $n = 2^{20}$, the C kernel's execution time of 0.9 ms is slightly faster than the Assembly kernel's time of 1.1 ms, resulting in a small discrepancy of 0.2 ms. As the input size escalates to $n = 2^{24}$, the C kernel's time of 13.66667 ms is faster to an extent in comparison to the Assembly kernel's time of 15.3 ms, a minor time difference of 1.63333 ms. At $n = 2^{28}$, the C kernel's execution time of 212.9 ms contrasted with the Assembly kernel's execution time of 246.73333 ms shows a moderate time difference of 33.83333 ms. For $n = 2^{30}$, though, the C kernel averaged 53279.73333 ms or 53.27973 seconds while the Assembly kernel only averaged 3014.53333 or 3.01453 seconds, which is a huge time gap of 50265.2 ms or 50.2652 seconds. It highlights that for lower values of $n$, compiler optimizations in release mode can unexpectedly slow down linked Assembly code to the point that C code is noticeably faster by comparison, while for high values of $n$, Assembly code has an edge on compiler-optimized C code.

## Screenshots

Program output in debug mode for $n = 2^{28}$.

![Program output for n = 2^28, debug mode](./docs/output_debug_2_28.png)

Program output in release mode for $n = 2^{28}$.

![Program output for n = 2^28, release mode](./docs/output_release_2_28.png)
