# Exception Research

## Measurements v1

||Компилятор|Столбец 3|
|-|-|---|
|Последовательная|Запись в столбце 2|Запись в столбце 3|
|Параллельная|Запись в столбце 2|Запись в столбце 3|

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
