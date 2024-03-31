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

In terms of the results of the execution times using the C kernel, the table above shows that in the smallest value of $n = 2^{20}$, C had a slightly faster average execution time of $2.13333$ ms in release mode than the average execution time of $3.66667$ ms in debug mode. The two values slightly differ by $1.53333$ ms. When we attempted to increase the value of $n$ to $2^{24}$, the mean execution time in release mode increased to $31.16667$ ms, while the mean execution time in debug mode rose to a higher value of $44.93333$ ms. Evidently, the difference between the two modes for the C kernel grew into a significant $13.76667$ ms, highlighting the effect of the growth of the number of elements for each vector. Meanwhile, in the largest value of $n = 2^{30}$, the average execution times grew exponentially at $59999.73333$ ms for the debug mode and $53279.73333$ ms for the release mode, which converts into an almost one-minute average running time for each mode. The gap between the two modes also increased massively at $6720$ ms, which converts into more than six seconds. As evident in the faster mean execution times for all three values of $n$ in release mode, it is clear that the compiler optimizations provided by the release mode can significantly improve the performance of the C kernel as compared to the debug mode.

On the other hand, the results of the execution times of the Assembly kernel show a slightly different behavior than the C kernel. For $n = 2^{20}$, the average execution time in release mode was marginally faster at $3.46667$ ms compared to the $3.8$ ms in debug mode. Comparatively, the two values slightly differ by $0.33333$ ms. Meanwhile, for $n = 2^{24}$, the release mode seems to have worsened the average execution time of the Assembly kernel at $55.9$ ms compared to the slightly faster $47.83333$ ms of the debug mode, with a difference of $8.06667$ ms. This behavior could be potentially caused by the optimizations of the release mode that prove ineffective for the Assembly kernel on smaller values of $n$. In general, the release mode should have applied compiler optimizations in the execution of the kernels, which in theory, can expedite the execution times. However, the compiler settings applied may have affected the Assembly kernel differently for smaller values of $n$ since its real effect was more evident in the higher value of $n = 2^{30}$. In the table, the Assembly kernel had an average execution time of $3014.53333$ ms in release mode, which is significantly faster than the $5253.7$ ms execution time in debug mode.

Comparing the results of the C kernel and the Assembly kernel side by side, the C kernel produced faster execution times on the two lower values of $n = 2^{20}$ and $n = 2^{24}$. In debug mode, C had an average execution time of $3.66667$ ms for the lowest value of $n$, while Assembly had a slightly worse average execution time of $3.8$ ms. The release mode makes their performance difference on small values of $n$ more apparent given that C had a $2.13333$ ms average execution time, while Assembly had a slightly slower $3.46667$ ms average execution time. The same behavior can be observed for $n = 2^{24}$ given the $44.93333$ ms and $47.83333$ ms mean execution times for C and Assembly, respectively, in debug mode. Likewise, the better performance of C was significantly highlighted in the release mode at $31.16667$ ms, while Assembly had an unexpected $55.9$ ms average execution time, with a jaw-dropping $24.73333$ ms difference. 

In theory, the Ass These values tell us that C can perform faster than Assembly on relatively lower values of $n$, and there are multiple possible factors to this. The first one is the compiler options (both debug and release mode) that can be more advantageous to the C kernel than the Assembly kernel, making its execution time faster than the Assembly. 


Moreover, the table indicates that in debug mode for $n = 2^{20}$, the C kernel's execution time of 2.96667 ms is slower than the Assembly kernel's time which clocks in at 0.9 ms, a modest discrepancy of 2.06667 ms. As the input size increases to $n = 2^{24}$, the C kernel's time of 41.43333 ms becomes even slower compared to the Assembly kernel's time of 10.53333 ms, with a much more pronounced gap of 30.9 ms. For $n = 2^{28}$, the C kernel's execution time of 675.53333 ms in contrast with the Assembly kernel's execution time of 176.13333 ms creates a substantial time difference of 499.4 ms. At $n = 2^{30}$, the C kernel's runtime of 60136.4 ms or 60.1364 seconds compared to the Assembly kernel's runtime of 5361.36667 ms or 5.36137 seconds results in a gigantic time difference of 54775.03333 ms or 54.77503 seconds. This shows that in debug mode, Assembly code can be significantly faster compared to C code.

However, the table further reveals that in release mode, for $n = 2^{20}$, the C kernel's execution time of 0.9 ms is slightly faster than the Assembly kernel's time of 1.1 ms, resulting in a small discrepancy of 0.2 ms. As the input size escalates to $n = 2^{24}$, the C kernel's time of 13.66667 ms is faster to an extent in comparison to the Assembly kernel's time of 15.3 ms, a minor time difference of 1.63333 ms. At $n = 2^{28}$, the C kernel's execution time of 212.9 ms contrasted with the Assembly kernel's execution time of 246.73333 ms shows a moderate time difference of 33.83333 ms. For $n = 2^{30}$, though, the C kernel averaged 53279.73333 ms or 53.27973 seconds while the Assembly kernel only averaged 3014.53333 or 3.01453 seconds, which is a huge time gap of 50265.2 ms or 50.2652 seconds. It highlights that for lower values of $n$, compiler optimizations in release mode can unexpectedly slow down linked Assembly code to the point that C code is noticeably faster by comparison, while for high values of $n$, Assembly code has an edge on compiler-optimized C code.

## Screenshots

Program output in debug mode for $n = 2^{28}$.

![Program output for n = 2^28, debug mode](./docs/output_debug_2_28.png)

Program output in release mode for $n = 2^{28}$.

![Program output for n = 2^28, release mode](./docs/output_release_2_28.png)
