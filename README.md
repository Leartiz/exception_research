# Exception Research

## Measurements v1

### Program: *cmd*
- See [here](./program/cmd/main.cpp)

<table>
    <!-- header -->
    <tr>
        <td rowspan="2"> 
            <b>№</b>
        </td>
        <td rowspan="2">
            <b>Type</b>
        </td>
        <td rowspan="2">
            <div align="center">
                <b>
                    Quantity:<br>
                    launches/<br> 
                    equations/<br>
                    exceptions
                </b>
            </div>
        </td>
        <td rowspan="1" colspan="2">
            <b>No exception/Normal/FullException (us)</b>
        </td>
    </tr>
    <tr align="center">
        <td>
            MinGW 11.2.0
        </td>
        <td>
            MSVC 17.3.32901.215
        </td>
    </tr>
    <!-- body -->
    <!-- next -->
    <tr align="center">
        <td rowspan="2"> 
            1
        </td>
        <td rowspan="1"> 
            ---
        </td>
        <td rowspan="2"> 
            5<br> <!-- launches -->
            3'000'000<br> <!-- equations -->
            1<br> <!-- exceptions -->
        </td>
        <!-- mingw -->
        <td rowspan="2"> 
            107945<br>
            108879<br>
            4414310<br>
        </td>
        <!-- msvc -->
        <td rowspan="2"> 
            1206528<br>
            1231119<br>
            13197281<br>
        </td>
    </tr>
    <tr align="center">
        <td rowspan="1"> 
            |||
        </td>
    </tr>
    <!-- next -->
        <tr align="center">
        <td rowspan="2"> 
            2
        </td>
        <td rowspan="1"> 
            ---
        </td>
        <td rowspan="2"> 
            -
        </td>
        <td rowspan="2"> 
            -
        </td>
        <td rowspan="2"> 
            -
        </td>
    </tr>
    <tr align="center">
        <td rowspan="1"> 
            |||
        </td>
    </tr>
</table>

## Experiments


### cpp

#### + OpenMP
- n1 - `parallel [num_threads]; critical` [here](./experiments/n1/main.cpp)
- n2 - `parallel [num_threads]; critical; nested` [here](./experiments/n2/main.cpp)
- n3 - one thread - one file [here](./experiments/n3/main.cpp)
- n4 - exception from thread [here](./experiments/n4/main.cpp)
- n5 - `omp_get_wtime` [here](./experiments/n5/main.cpp)
- n6 - `omp_get_wtime; omp_get_wtick` [here](./experiments/n6/main.cpp)
- n7 - `_OPENMP` [here](./experiments/n7/main.cpp)

#### std
- n8 - initializer list or constructor [here](./experiments/n8/main.cpp)
- n9 - vector/error creation time [here](./experiments/n9/main.cpp)
- n10 - code analysis with exceptions [here](./experiments/n10/main.cpp)
- n11 - vector/error throw time [here](./experiments/n11/main.cpp)
