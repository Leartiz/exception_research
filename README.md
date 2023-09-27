# Exception Research

|Столбец 1|Столбец 2|Столбец 3|
|-|--------|---|
|Длинная запись в первом столбце|Запись в столбце 2|Запись в столбце 3|
|Кртк зпс| |Слева нет записи|

## Experiments

### OpenMP
- n1 - `parallel [num_threads]; critical` [here](./experiments/n1/main.cpp)
- n2 - `parallel [num_threads]; critical; nested` [here](./experiments/n2/main.cpp)
- n3 - one thread - one file [here](./experiments/n3/main.cpp)
- n4 - exception from thread [here](./experiments/n4/main.cpp)
- n5 - `omp_get_wtime` [here](./experiments/n5/main.cpp)
- n6 - `omp_get_wtime; omp_get_wtick` [here](./experiments/n6/main.cpp)
- n7 - `_OPENMP` [here](./experiments/n7/main.cpp)